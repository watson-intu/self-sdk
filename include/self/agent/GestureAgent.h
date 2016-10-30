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

#ifndef SELF_GESTURE_AGENT_H
#define SELF_GESTURE_AGENT_H

#include "IAgent.h"
#include "blackboard/Gesture.h"
#include "SelfLib.h"

class SkillInstance;

class SELF_API GestureAgent : public IAgent
{
public:
	RTTI_DECL();

	GestureAgent()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:

	//! Event Handlers
	void 		OnGesture(const ThingEvent & a_ThingEvent);
};

#endif // SELF_GESTURE_AGENT_H

