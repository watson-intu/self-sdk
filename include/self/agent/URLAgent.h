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

#ifndef SELF_URL_AGENT_H
#define SELF_URL_AGENT_H

#include "IAgent.h"
#include "blackboard/URL.h"
#include "blackboard/Text.h"
#include "services/SpeechToText/SpeechToText.h"
#include "sensors/TouchData.h"
#include "sensors/SensorManager.h"
#include "sensors/ISensor.h"
#include "services/URLService.h"
#include "SelfLib.h"

class SkillInstance;

//! This agent handles displaying the URL (e.g. WayBlazer)
//! It acts on a URL object placed on the blackboard. It sends the URL
//! using a service to a system for displaying the URL.

class SELF_API URLAgent : public IAgent
{
public:
	RTTI_DECL();

	URLAgent() : m_HeartBeatServerInterval(15)
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

	void OnHeartBeatTimer();

private:
	//! Data
	int m_HeartBeatServerInterval;
	TimerPool::ITimer::SP m_HeartBeatTimer;
	std::vector<std::string> m_FailureResponses;

	//! Event Handlers
	void 		OnURL(const ThingEvent & a_ThingEvent);
	void        OnResponse( URLService::URLServiceData * a_UrlAgentData );

};


#endif // URL Agent
