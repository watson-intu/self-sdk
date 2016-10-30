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

#ifndef SELF_SLEEP_AGENT_H
#define SELF_SLEEP_AGENT_H

#include "IAgent.h"
#include "SelfLib.h"

class SELF_API SleepAgent : public IAgent
{
public:
	RTTI_DECL();

	SleepAgent() : 
		m_WakeTime( 3 * (60 * 60) ), 
		m_SleepTime( 30 * 60 ), 
		m_bSleeping( false )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Data
	double m_WakeTime;					// how many seconds we can run before we sleep
	double m_SleepTime;					// how long do we sleep
	bool m_bSleeping;

	std::vector<std::string> m_HealthSensorMasks;

	TimerPool::ITimer::SP m_spSleepTimer;
	TimerPool::ITimer::SP m_spWakeTimer;

	//! Event Handlers
	void 		OnHealth(const ThingEvent & a_ThingEvent);
	void		OnSleep();
	void		OnWake();
};


#endif // URL Agent
