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

#ifndef WDC_IWEB_SOCKET_H
#define WDC_IWEB_SOCKET_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>

#include "RTTI.h"
#include "Delegate.h"
#include "WDCLib.h"

//! Common interface for sending and receiving data on a web socket.
class WDC_API IWebSocket : public boost::enable_shared_from_this<IWebSocket>
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<IWebSocket>			SP;
	typedef boost::weak_ptr<IWebSocket>				WP;

	enum OpCode
	{
		CONTINUATION = 0x0,
		TEXT_FRAME = 0x1,
		BINARY_FRAME = 0x2,
		CLOSE = 8,
		PING = 9,
		PONG = 0xa,
	};

	struct Frame
	{
		Frame() : m_Op( CONTINUATION )
		{}

		OpCode				m_Op;
		std::string			m_Data;
		WP					m_wpSocket;
	};
	typedef boost::shared_ptr<Frame>			FrameSP;

	virtual void SetFrameReceiver( Delegate<FrameSP> a_Receiver ) = 0;
	virtual void SetErrorHandler( Delegate<IWebSocket *> a_Handler ) = 0;
	virtual void SendBinary(const std::string & a_Binary) = 0;
	virtual void SendText(const std::string & a_Text) = 0;
	virtual void SendPing(const std::string & a_PingData) = 0;
	virtual void SendPong(const std::string & a_PingData) = 0;
	virtual void SendClose(const std::string & a_Reason) = 0;
	virtual bool Close() = 0;
};

#endif
