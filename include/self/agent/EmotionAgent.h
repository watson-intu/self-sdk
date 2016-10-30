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

#ifndef SELF_EMOTION_AGENT_H
#define SELF_EMOTION_AGENT_H

#include "IAgent.h"
#include "blackboard/Emotion.h"
#include "sensors/SensorManager.h"
#include "sensors/MoodData.h"
#include "SelfLib.h"

class SkillInstance;

//! This agent handles emotions

class SELF_API EmotionAgent : public IAgent
{
public:
	RTTI_DECL();

	EmotionAgent() : m_SaySomething(false), m_WaitTime( 30.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:

	//! Types
	typedef SensorManager::SensorList   SensorList;

	//! Data
	SensorList				m_MoodSensors;
	bool 					m_SaySomething;
	float 					m_WaitTime;
	TimerPool::ITimer::SP	m_spWaitTimer;

	//! Event Handlers
	void 		OnEmotion(const ThingEvent & a_ThingEvent);
	void        OnMoodData(IData * data);
	void		OnEnableEmotion();

};

#endif // EMOTION Agent

