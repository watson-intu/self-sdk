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

#ifndef SELF_PLAN_H
#define SELF_PLAN_H

#include <string>

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

#include "ICondition.h"
#include "IAction.h"
#include "SelfLib.h"

class Goal;

//! This class contains the code/data for a single plan instance. A plan has pre-conditions, actions, and post-conditions. 
//! pre-conditions are checked before a plan can be considered for execution.
//! Actions make up the actual plan. A action is a polymorphic type that can do any number of actions (e.g. add a goal, use a skill, etc)
//! post-conditions are checked once a plan completes to see if the plan succeeded or failed.
class SELF_API Plan : public ISerializable, public boost::enable_shared_from_this<Plan>
{
public:
	RTTI_DECL();

	//! Types
	typedef std::vector< ICondition::SP >		Conditions;
	typedef std::vector< IAction::SP >			Actions;
	
	typedef boost::shared_ptr<Plan>				SP;
	typedef boost::weak_ptr<Plan>				WP;

	//! Construction
	Plan();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	const std::string & GetPlanId() const;								// unique ID for this plan
	const Conditions & GetPreConditions() const;					// pre conditions needed to execute this plan
	const Actions & GetActions() const;								// actions that make up this plan
	const Conditions & GetPostConditions() const;					// post conditions to determine if the plan was a success or not

	//! Mutators
	void SetPlanId(const std::string & a_PlanId);
	void SetPreConditions( const Conditions & a_Conditions );
	void SetActions( const Actions & a_Actions );					// set the actions of this plan
	void SetPostConditions( const Conditions & a_Conditions );

	//! Check if this plan is a valid plan for the given goal object, returns a value between 0.0 (no) to 1.0 (yes)
	float TestPreConditions( Goal::SP a_spGoal );	
	float TestPostConditions( Goal::SP a_spGoal );

private:
	//! Data
	std::string		m_PlanId;
	Conditions		m_PreConditions;
	Actions			m_Actions;
	Conditions		m_PostConditions;
};

//----------------------------------

inline const std::string & Plan::GetPlanId() const
{
	return m_PlanId;
}

inline const Plan::Conditions & Plan::GetPreConditions() const
{
	return m_PreConditions;
}

inline const Plan::Actions & Plan::GetActions() const
{
	return m_Actions;
}

inline const Plan::Conditions & Plan::GetPostConditions() const
{
	return m_PostConditions;
}

inline void Plan::SetPlanId(const std::string & a_PlanId)
{
	m_PlanId = a_PlanId;
}

inline void Plan::SetPreConditions(const Conditions & a_Conditions)
{
	m_PreConditions = a_Conditions;
}

inline void Plan::SetActions(const Actions & a_Actions)
{
	m_Actions = a_Actions;
}

inline void Plan::SetPostConditions(const Conditions & a_Conditions)
{
	m_PostConditions = a_Conditions;
}

#endif
