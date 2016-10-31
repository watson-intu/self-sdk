/**
* Copyright 2016 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef WDC_WEB_CLIENT_H
#define WDC_WEB_CLIENT_H

#include <map>

#include "boost/asio.hpp"		// not including SSL at this level on purpose
#include "boost/thread.hpp"
#include "boost/thread/mutex.hpp"

#if defined(ENABLE_OPENSSL_INCLUDES)
#include "boost/asio/ssl.hpp"
#else

// boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *
namespace boost {
	namespace asio {
		namespace ssl{
			template<class SOCKET>
			class stream
			{};
			class context
			{};
		};
	};
};

#endif
#include "boost/atomic.hpp"

#include "Delegate.h"
#include "StringUtil.h"
#include "URL.h"
#include "IWebClient.h"
#include "WebSocketFramer.h"
#include "TimerPool.h"
#include "WDCLib.h"		// include last always


//! Connector object for abstracting a socket connection through boost::asio,
//! this class handles HTTP, HTTPS, WS, and WSS connection types.
//! This class depends on the boost::asio classes

//! All public functions of this class should be invoked from the main thread and will not block.
class WDC_API WebClient : public IWebClient
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<WebClient>		SP;
	typedef boost::weak_ptr<WebClient>			WP;

	//! Construction
	WebClient();
	~WebClient();

	//! IWebSocket interface
	virtual void SetFrameReceiver( Delegate<FrameSP> a_Receiver );
	virtual void SetErrorHandler( Delegate<IWebSocket *> a_Handler);
	virtual void SendBinary(const std::string & a_Binary);
	virtual void SendText(const std::string & a_Text);
	virtual void SendPing(const std::string & a_PingData);
	virtual void SendPong(const std::string & a_PingData);
	virtual void SendClose(const std::string & a_Reason);

	//! IWebClient interface
	virtual SocketState GetState() const;
	virtual const URL & GetURL() const;
	virtual const Headers & GetHeaders() const;
	virtual void SetURL( const URL & a_URL );
	virtual void SetStateReceiver(Delegate<IWebClient *> a_StateReceiver );
	virtual void SetDataReceiver(Delegate<RequestData *> a_DataReceiver);

	virtual void SetHeader( const std::string & a_Key, 
		const std::string & a_Value );
	virtual void SetHeaders( const Headers & a_Headers, bool a_bMerge = false );
	virtual void SetRequestType( const std::string & a_ReqType );		// GET, POST, DELETE
	virtual void SetBody( const std::string & a_Body );
	virtual bool Send();

	virtual bool Close();
	virtual bool Shutdown();

	virtual bool Request( const URL & a_URL, 
		const Headers & a_Headers,
		const std::string & a_RequestType, 
		const std::string & a_Body,
		Delegate<RequestData *> a_DataReceiver,
		Delegate<IWebClient *> a_StateReceiver );

	static void SetClientId( const std::string & a_ClientId );
	static const std::string & GetClientId();

	SP shared_from_this()
	{
		return boost::static_pointer_cast<WebClient>( IWebClient::shared_from_this() );
	}

private:
	//! Private Functions
	void		SetState( SocketState a_eState );
	void		BeginConnect();
	void		HandleConnect( const boost::system::error_code & error, 
					boost::asio::ip::tcp::resolver::iterator i );
	void		HandleHandShake( const boost::system::error_code & error );

	void		OnConnected();
	void		HTTP_RequestSent( RequestData * a_pReq, const boost::system::error_code& error,
					size_t bytes_transferred);
	void		HTTP_ReadHeaders( RequestData * a_pReq, const boost::system::error_code& error,
					size_t bytes_transferred);
	void		HTTP_ReadContent(RequestData * a_pReq, const boost::system::error_code& error,
					size_t bytes_transferred);

	void		WS_Read( RequestData * a_pReq, const boost::system::error_code & error,
					size_t bytes_transferred );
	void		WS_Send( OpCode opcode, const std::string & a_Data, bool a_bUseMask = true );
	void		WS_QueueSend(std::string * a_pBuffer);
	void		WS_SendNext();
	void		WS_Sent( const boost::system::error_code& error,
					size_t bytes_transferred, std::string * a_pBuffer );

	void		OnResponse( RequestData * a_pData );
	void		OnWebSocketFrame( IWebSocket::Frame * a_pData );
	void		OnClose();
	void		OnDisconnected();
	void		Cleanup();

	//! Types
	typedef std::list<std::string *>		BufferList;

	//! Data
	SocketState		m_eState;
	URL				m_URL;
	Headers			m_Headers;
	std::string		m_RequestType;
	std::string		m_Body;

	//! WebSocket data
	bool			m_WebSocket;			// set to true if we have a web socket
	Delegate<IWebClient *>
					m_StateReceiver;
	Delegate<RequestData *>
					m_DataReceiver;
	Delegate<FrameSP>
					m_OnFrame;
	Delegate<IWebSocket *>
					m_OnError;

	boost::asio::ip::tcp::socket *
					m_pSocket;
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *
					m_pStream;
	boost::asio::ssl::context *
					m_pSSL;

	std::string		m_Request;				// the sent request 
	boost::asio::streambuf
					m_Response;				// response buffer
	std::string		m_Incoming;				// received web socket data
	BufferList		m_Pending;				// pending sends
	BufferList		m_Send;					// send queue
	bool			m_bChunked;
	size_t			m_ContentLen;

	volatile bool	m_SendError;			// set to true when a send fails
	boost::atomic<size_t>
					m_SendCount;			// number of outstanding websocket sends
	boost::recursive_mutex
					m_SendLock;

	static std::string 
					sm_ClientId;			// ID placed in all headers sent by this client
};

inline WebClient::SocketState WebClient::GetState() const
{
	return m_eState;
}

inline const URL &  WebClient::GetURL() const
{
	return m_URL;
}

inline const WebClient::Headers &  WebClient::GetHeaders() const
{
	return m_Headers;
}

inline void WebClient::SetURL(const URL & a_URL)
{
	m_URL = a_URL;
}

inline void WebClient::SetStateReceiver(Delegate<IWebClient *> a_StateReceiver)
{
	m_StateReceiver = a_StateReceiver;
}

inline void WebClient::SetDataReceiver(Delegate<RequestData *> a_Receiver)
{
	m_DataReceiver = a_Receiver;
}

inline void WebClient::SetHeader(const std::string & a_Key,
	const std::string & a_Value)
{
	m_Headers[ a_Key ] = a_Value;
}

inline void WebClient::SetHeaders(const Headers & a_Headers, bool a_bMerge /*= false*/ )
{
	if ( a_bMerge )
	{
		for( Headers::const_iterator iHeader = a_Headers.begin(); iHeader != a_Headers.end(); ++iHeader )
			m_Headers[iHeader->first] = iHeader->second;
	}
	else
		m_Headers = a_Headers;
}

inline void WebClient::SetRequestType(const std::string & a_ReqType)
{
	m_RequestType = a_ReqType;
}

inline void WebClient::SetBody( const std::string & a_Body )
{
	m_Body = a_Body;
}

//----------------------------------------------------

class WDC_API WebClientService
{
public:
	static WebClientService * Instance();

	WebClientService();
	~WebClientService();

	boost::asio::io_service & GetService()
	{
		return m_Service;
	}

private:
	boost::asio::io_service	m_Service;
	boost::asio::io_service::work
							m_Work;
	boost::thread			m_ServiceThread;
	TimerPool::ITimer::SP	m_spStatsTimer;

	static WebClientService *
							sm_pInstance;

	void OnDumpStats();
};


#endif

