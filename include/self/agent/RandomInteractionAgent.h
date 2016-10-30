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

#ifndef SELF_RANDOMINTERACTIONAGENT_H
#define SELF_RANDOMINTERACTIONAGENT_H

#include <list>

#include "IAgent.h"
#include "utils/Factory.h"
#include "utils/ThreadPool.h"

#include "SelfLib.h"


//! Forward Declarations
class SelfInstance;

class SELF_API RandomInteractionAgent : public IAgent
{
public:
	RTTI_DECL();

	//! Construction
	RandomInteractionAgent() : m_fMinSpeakDelay( 60.0f ), m_fMaxSpeakDelay( 120.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Data
	float						m_fMinSpeakDelay;
	float						m_fMaxSpeakDelay;
	std::string					m_RandomInteractionUtterance;

	TimerPool::ITimer::SP		m_spSpeakTimer;

	//! Event Handlers
	void OnInteraction(const ThingEvent &a_ThingEvent);
	void StartTimer();
	void OnSpeak();
};

#endif //SELF_RANDOMINTERACTIONAGENT_H
