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

#ifndef SELF_GOAL_AGENT_H
#define SELF_GOAL_AGENT_H

#include <list>
#include <algorithm>

#include "IAgent.h"
#include "planning/PlanInstance.h"
#include "blackboard/ThingEvent.h"
#include "SelfLib.h"

class Goal;

//! This goal manager watches for goals on the blackboard, and creates the plans
//! for those goals. It tracks those plans and updates the goals as the plan state changes.
class SELF_API GoalAgent : public IAgent
{
public:
	RTTI_DECL();

	//! Construction
    GoalAgent();
    ~GoalAgent();

	//! IAgent interface
	const char * GetName() const;
	bool OnStart();				
	bool OnStop();				

private:
	//! Callback for a new goal getting added to the blackboard
	void OnGoal(const ThingEvent &a_ThingEvent);
	//! Callback for a plan instance
	void OnPlanStateChanged(PlanInstance::SP a_spPlan);

	//! Types
	typedef std::set<PlanInstance::SP>	PlanSet;

	//! Data
	PlanSet			m_ActivePlans;
	Goal::SP		m_spPlanningGoal;
};

#endif //SELF_GOAL_AGENT_H
