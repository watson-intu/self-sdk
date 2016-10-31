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

#ifndef SELF_TELEPHONY_AGENT_H
#define SELF_TELEPHONY_AGENT_H

#include "IAgent.h"
#include "blackboard/Confirm.h"
#include "sensors/TelephonyMicrophone.h"
#include "gestures/TelephonySpeechGesture.h"
#include "services/Telephony/Telephony.h"
#include "SelfLib.h"

//! This agent handles making and answering phone calls
class SELF_API TelephonyAgent : public IAgent
{
public:
	RTTI_DECL();

	TelephonyAgent();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Data
	std::string                 m_TelephonyUtterance;
	std::string                 m_FromNumber;
	std::string                 m_ToNumber;
	std::string                 m_SmsNumber;
	std::string                 m_ToName;
	std::string                 m_TextMessage;
	std::string                 m_IncomingCallMessage;

	Telephony *                 m_pTelephony;
	unsigned int                m_AudioSent;
	unsigned int                m_AudioRecv;
	bool						m_bInCall;

	TelephonyMicrophone::SP     m_pTelephonyMicrophone;
	TelephonySpeechGesture::SP  m_pTelephonySpeechGesture;

	void                        BeginCall();
	void                        EndCall();

	//! Event Handlers
	void                        OnTelephonyIntent(const ThingEvent & a_ThingEvent);
	void                        OnCommand( const Json::Value & a_Call );
	void                        OnAudioOut( const std::string & a_Data );
	void                        OnAcceptIncomingCall( Confirm::SP a_spConfirm );

};

#endif // SELF_TELEPHONY_AGENT_H