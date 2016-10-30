/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#ifndef REST_GESTURE_H
#define REST_GESTURE_H

// forward declare 
namespace happyhttp { class Response; }

#include "IGesture.h"
#include "utils/IWebClient.h"
#include "SelfLib.h"

//! This gesture is for making a REST call to a server.
class SELF_API RestGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Types
	typedef IWebClient::Headers		Headers;

	//! Construction
	RestGesture() : m_Type( "GET" ), m_bAsync( false )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

	void SetURL( const std::string & a_URL )
	{
		m_URL = a_URL;
	}
	void SetRequestType( const std::string & a_Type )
	{
		m_Type = a_Type;
	}
	void SetHeaders( const Headers & a_Headers )
	{
		m_Headers = a_Headers;
	}
	void SetBody( const std::string & a_Body )
	{
		m_Body = a_Body;
	}
	void SetUserName( const std::string & a_UserName )
	{
		m_UserName = a_UserName;
	}
	void SetPassword( const std::string & a_Password )
	{
		m_Password = a_Password;
	}
	
protected:
	//! Data
	std::string			m_URL;			// url of the rest request
	std::string			m_Type;			// GET, POST, or DELETE
	Headers				m_Headers;		// headers for the request
	std::string			m_Body;			// body of the request
	std::string			m_UserName;
	std::string			m_Password;
	bool				m_bAsync;

	struct Context 
	{
		Context() : m_Error( false )
		{}

		GestureDelegate		m_Callback;
		ParamsMap			m_Params;
		std::string			m_Response;
		bool				m_Error;

	};

	void OnData( IWebClient::RequestData * a_pData );
	void OnState( IWebClient * a_pClient );
};


#endif //IGESTURE_H
