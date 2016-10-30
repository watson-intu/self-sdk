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

#ifndef WDC_IWEB_SERVER_H
#define WDC_IWEB_SERVER_H

#include <map>
#include <boost/shared_ptr.hpp>

#include "Delegate.h"
#include "StringUtil.h"
#include "TimerPool.h"
#include "Log.h"
#include "RTTI.h"
#include "IWebSocket.h"
#include "WDCLib.h"	

//! Server class for handling incoming REST requests and WebSocket connections. 
class WDC_API IWebServer : public boost::enable_shared_from_this<IWebServer>
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<IWebServer>			SP;
	typedef boost::weak_ptr<IWebServer>				WP;

	typedef std::map< std::string, std::string, StringUtil::ci_less >		
		Headers;

	class IConnection : public IWebSocket
	{
	public:
		//! Types
		typedef boost::shared_ptr<IConnection>		SP;
		typedef Delegate< SP >						FrameCallback;

		//! Interface
		virtual bool IsClosed() const = 0;
		virtual bool IsWebSocket() const = 0;

		virtual void SendAsync(const std::string & a_Send) = 0;
		virtual void ReadAsync(size_t a_Bytes, Delegate< std::string * > a_ReadCallback ) = 0;
		virtual void SendResponse(int a_nStatusCode, const std::string & a_Reply, const Headers & a_Headers,
			const std::string & a_Content, bool a_bClose = true ) = 0;
		virtual void SendResponse(int a_nStatusCode, const std::string & a_Reply, 
			const std::string & a_Content, bool a_bClose = true ) = 0;
		virtual void StartWebSocket(const std::string & a_WebSocketKey ) = 0;

		SP shared_from_this()
		{
			return boost::static_pointer_cast<IConnection>( IWebSocket::shared_from_this() );
		}
	};
	typedef boost::shared_ptr<IConnection>		ConnectionSP;

	//! This object is created when a request is received from a connected client.
	struct WDC_API Request
	{
	public:
		//! Types
		std::string				m_Origin;		// remote IP of the request
		std::string				m_RequestType;	// GET, POST, DELETE, etc..
		std::string				m_EndPoint;		// end-point in the request
		std::string				m_Protocol;		// HTTP/1.1
		Headers					m_Headers;		// parsed headers
		ConnectionSP			m_spConnection;	// the connection object for this request
	};
	typedef boost::shared_ptr<Request>			RequestSP;

	// make destructor virtual to ensure proper cleanup
	virtual ~IWebServer()
	{}

	static IWebServer * Create( const std::string & a_Interface = std::string(),
		int a_nPort = 80, int a_nThreads = 5, float a_fRequestTimeout = 30.0f );
	static IWebServer * Create(const std::string & a_CertFile,
		const std::string & a_PrivateKeyFile,
		const std::string & a_VerifyFile = std::string(),
		const std::string & a_Interface = std::string(),
		int a_nPort = 443, int a_nThreads = 5, float a_fRequestTimeout = 30.0f);

	//! This starts this server listening for incoming connections
	virtual bool Start() = 0;
	//! Invoke to shutdown this server.
	virtual bool Stop() = 0;

	//! Add an end-point to this server, the provided delegate will be invoked with the 
	//! Request object when an incoming client connections makes a request that matches 
	//! the provided end-point mask.
	//! If a_bInvokeOnMain is false the delegate will be invoked in a thread from this servers thread-pool.
	virtual void AddEndpoint(const std::string & a_EndPointMask,
		Delegate<RequestSP> a_RequestHandler,
		bool a_bInvokeOnMain = true ) = 0;
	//! Remove a register end-point.
	virtual bool RemoveEndpoint(const std::string & a_EndPointMask) = 0;

protected:
	//! Accept incoming connections, this must be provided by the base class.
	virtual void Accept() = 0;
};

#endif

