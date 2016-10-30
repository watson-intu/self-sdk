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

#ifndef SELF_PLAN_INSTANCE_H
#define SELF_PLAN_INSTANCE_H

#include <set>
#include <list>

#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

#include "utils/ParamsMap.h"
#include "utils/Delegate.h"
#include "Plan.h"

#include "SelfLib.h"		// include last always

class PlanManager;

//! This object is created to use a skill through the SkillManager.
class SELF_API PlanInstance : public boost::enable_shared_from_this<PlanInstance>
{
public:
	//! Types
	enum PlanState {
		INVALID,
		FINDING,				// we are searching for the plan remotely
		EXECUTING,				// we are executing the plan
		COMPLETED,				// plan is completed
		FAILED,					// we failed to execute the plan
		UNAVAILABLE,			// the plan was not found
		ABORTED					// the plan was aborted
	};
	typedef boost::shared_ptr<PlanInstance>		SP;
	typedef boost::weak_ptr<PlanInstance>		WP;
	typedef Delegate<SP>						StateCallback;

	//! Construction
	PlanInstance( PlanManager * a_pManager, Goal::SP a_spGoal, StateCallback a_StateCallback );
	~PlanInstance();

	//! Accessors
	PlanManager *			GetManager() const;
	const std::string &		GetInstanceId() const;
	Goal::SP				GetGoal() const;
	const std::string &		GetPlanId() const;
	Plan::SP				GetPlan() const;
	PlanState				GetState() const;

	//! Mutators
	bool					Start();
	bool					Abort();			// attempt to stop this plan, returns true if abort worked

	//! Static
	static const char *		GetPlanStateText(PlanState a_eState);

private:
	void					StartPlan( Plan::SP a_spPlan );
	void					NextAction();
	void					OnActionResult( const IAction::State & a_Result );
	void					ChangeState( PlanState a_eState );

	//! Types
	typedef std::list<IAction::SP>		ActionList;
	typedef std::set<IAction::SP>		ActionSet;

	//! Data
	PlanManager *			m_pManager;
	std::string				m_InstanceId;
	Goal::SP				m_spGoal;
	Plan::SP				m_spPlan;
	PlanState				m_eState;
	ActionList				m_Actions;
	ActionSet				m_ActiveActions;
	StateCallback			m_StateCallback;

	SP						m_spThis;			// pointer to for keeping this object in memory while waiting for callbacks.
};

//----------------------------------------------------

inline PlanManager * PlanInstance::GetManager() const
{
	return m_pManager;
}

inline const std::string & PlanInstance::GetInstanceId() const
{
	return m_InstanceId;
}

inline Goal::SP PlanInstance::GetGoal() const
{
	return m_spGoal;
}

inline const std::string & PlanInstance::GetPlanId() const
{
	return m_spPlan ? m_spPlan->GetPlanId() : EMPTY_STRING;
}

inline Plan::SP PlanInstance::GetPlan() const
{
	return m_spPlan;
}

inline PlanInstance::PlanState PlanInstance::GetState() const
{
	return m_eState;
}


#endif
