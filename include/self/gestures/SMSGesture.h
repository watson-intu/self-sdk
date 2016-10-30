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

#ifndef SELF_SMS_GESTURE_H
#define SELF_SMS_GESTURE_H

#include "services/IService.h"
#include "IGesture.h"
#include "SelfLib.h"

//! This gesture can use the SMS service to send a text message.
class SELF_API SMSGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	SMSGesture() 
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

protected:
	void Send();
	void OnSent( IService::Request * a_pRequest );

	//! Data
	std::string			m_Number;
	std::string			m_Message;
};


#endif // SELF_SMS_GESTURE_H
