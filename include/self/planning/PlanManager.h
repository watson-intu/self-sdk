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

#ifndef SELF_PLAN_MANAGER_H
#define SELF_PLAN_MANAGER_H

#include <string>
#include <map>
#include <set>

#include "utils/ISerializable.h"
#include "PlanInstance.h"
#include "SelfLib.h"				// include last

//! This manager manages all ISkill instances including the saving and loading of skills from local storage.
class SELF_API PlanManager : public ISerializable
{
public:
	RTTI_DECL();

	//! Types
	typedef std::list<std::string>					PlanFiles;
	typedef std::map< std::string, Plan::SP >		PlanMap;
	typedef Delegate<PlanInstance::SP>				StateCallback;

	//! Construction
	PlanManager();
	~PlanManager();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Find a plan by it's unique ID.
	Plan * FindPlan( const std::string & a_ID ) const;

	//! Start this manager
	bool Start( const PlanFiles & a_PlanFiles );
	//! Stop this manager
	bool Stop();

	//! Find a local plan based on the ability to complete the provided goal.
	Plan::SP SelectPlan( Goal::SP a_pGoal );
	//! Add a new plan into this manager, if a_bAddRemote is true then the plan is uploaded
	//! to the remote server as well.
	bool AddPlan( const Plan::SP & a_spPlan, bool a_bAddRemote = true );
	//! delete a plan object by it's pointer.
	bool DeletePlan( const Plan::SP & a_spPlan );
	//! delete a plan by it's ID
	bool DeletePlan( const std::string & a_ID );

	//! The main interface for executing a plan to complete the provided goal. This routine
	//! may generate a new plan to complete the given goal if no plans are found that are suitable
	//! for completing the given goal. The callback will be invoked each time the PlanInstance
	//! state is changed.
	PlanInstance::SP ExecutePlan( Goal::SP a_pGoal, StateCallback a_Callback );

private:
	//! Data
	bool			m_bActive;

	//! Serialized Data
	PlanMap			m_Plans;
	float			m_fMinPlanScore;			// the minimum pre-condition score required for a local plan to be considered for selection.
};

//----------------------------------------------

inline Plan * PlanManager::FindPlan( const std::string & a_ID ) const
{
	PlanMap::const_iterator iPlan = m_Plans.find( a_ID );
	if ( iPlan != m_Plans.end() )
		return iPlan->second.get();

	return NULL;
}

#endif
