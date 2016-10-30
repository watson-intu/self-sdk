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

#ifndef WDC_URL_H
#define WDC_URL_H

#ifndef WIN32
#include <stdlib.h>
#include <strings.h>
#define _stricmp strcasecmp
#endif

#include <string>		// std::string

#include "StringUtil.h"
#include "WDCLib.h"

//! Helper class for parsing a URL and extracting the different parts.
class WDC_API URL
{
public:
	static const std::string PROTOCOL_END;
	static const std::string PORT_BEGIN;
	static const std::string SEPERATOR;

	//! Construction
	URL() : m_Port( 0 )
	{}
	URL( const char * a_pURL ) : m_Port( 0 )
	{
		Parse( a_pURL );
	}
	URL( const std::string & a_URI ) : m_Port( 0 )
	{
		Parse( a_URI );
	}
	URL( const URL & a_Copy ) : m_Protocol( a_Copy.m_Protocol ),
		m_Host( a_Copy.m_Host ),
		m_Port( a_Copy.m_Port ),
		m_EndPoint( a_Copy.m_EndPoint )
	{}

	//! e.g. http, https, wss, ws, tcp, etc..
	const std::string & GetProtocol() const
	{
		return m_Protocol;
	}
	//! The hostname
	const std::string & GetHost() const
	{
		return m_Host;
	}
	//! The port, if not specific after the host name then we will return the default based on the protocol.
	int GetPort() const
	{
		return m_Port;
	}
	//! The end-point occurs after the hostname, it will NOT include the leading / character.
	const std::string & GetEndPoint() const
	{
		return m_EndPoint;
	}

	//! Get the fully qualified URL
	std::string GetURL() const
	{
		std::string url( m_Protocol + PROTOCOL_END + m_Host );
		AppendPort( url );
		AppendEndPoint( url );

		return url;
	}

	//! Returns the default port for the given protocol.
	static int GetProtocolPort( const std::string & a_Protocol )
	{
		if ( _stricmp( a_Protocol.c_str(), "http" ) == 0 || _stricmp( a_Protocol.c_str(), "ws" ) == 0 )
			return 80;
		else if ( _stricmp( a_Protocol.c_str(), "https" ) == 0 || _stricmp( a_Protocol.c_str(), "wss" ) == 0 )
			return 443;

		return 0;
	}

	void Parse( const std::string & a_URL )
	{
		Reset();

		m_Host = a_URL;

		size_t protocolEnd = m_Host.find( PROTOCOL_END );
		if ( protocolEnd != std::string::npos )
		{
			m_Protocol = m_Host.substr(0, protocolEnd);
			m_Host = m_Host.substr( protocolEnd + PROTOCOL_END.size() );
		}
		else
			m_Protocol = "http";

		m_Port = GetProtocolPort( m_Protocol );

		// look for the start of a port		
		size_t portStart = m_Host.find( PORT_BEGIN );
		if ( portStart != std::string::npos )
		{
			size_t portEnd = m_Host.find( SEPERATOR, portStart );
			if ( portEnd == std::string::npos )		// if no slash found, then go up to the end of the string..
				portEnd = m_Host.size() - 1;

			std::string port = m_Host.substr( portStart + 1 );
			m_Port = atoi( port.c_str() );
			if ( portEnd < (m_Host.size() - 1) )
				m_Host = m_Host.substr( 0, portStart ) + m_Host.substr( portEnd );
			else
				m_Host = m_Host.substr( 0, portStart );
		}

		size_t hostEnd = m_Host.find( SEPERATOR );
		if ( hostEnd != std::string::npos )
		{
			m_EndPoint = m_Host.substr( hostEnd + 1 );
			m_Host = m_Host.substr( 0, hostEnd );
		}
	}

	void SetProtocol(const std::string & a_Protocol)
	{
		m_Protocol = a_Protocol;
	}
	void SetHost(const std::string & a_Host)
	{
		m_Host = a_Host;
	}
	void SetPort(int a_Port)
	{
		m_Port = a_Port;
	}
	void SetEndPoint(const std::string & a_EndPoint)
	{
		m_EndPoint = a_EndPoint;
	}

	void Reset()
	{
		m_Protocol.clear();
		m_Host.clear();
		m_Port = 0;
		m_EndPoint.clear();
	}

private:
	//! Data
	std::string			m_Protocol;
	std::string			m_Host;
	int					m_Port;
	std::string			m_EndPoint;

	void AppendPort( std::string & a_Out ) const
	{
		if ( m_Port != 0 && m_Port != GetProtocolPort( m_Protocol ) )
			a_Out += StringUtil::Format( "%s%u", PORT_BEGIN.c_str(), m_Port );
	}
	void AppendEndPoint( std::string & a_Out ) const
	{
		if ( m_EndPoint.size() > 0 )
			a_Out += SEPERATOR + m_EndPoint;
	}
};

#endif

