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

#ifndef GOAL_H
#define GOAL_H

#include "skills/SkillManager.h"
#include "blackboard/IThing.h"
#include "blackboard/BlackBoard.h"
#include "utils/ParamsMap.h"
#include "SelfLib.h"			// include last always

//! This object represents a goal that is added by a agent onto the Blackboard. Once a goal
//! is added the GoalManager will pass the goal onto the PlanManager and track a PlanInstance
//! object until the goal is completed or failed.
class SELF_API Goal : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Goal>		SP;
	typedef boost::weak_ptr<Goal>		WP;

	//! Construction
	Goal() : IThing( TT_COGNITIVE, "ADDED" )
	{}
	Goal( const std::string & a_Name ) : 
		IThing( TT_COGNITIVE, "ADDED" ),
		m_Name( a_Name )
	{}
	Goal( const std::string & a_Name, const ParamsMap & a_Params ) : 
		IThing( TT_COGNITIVE, "ADDED" ),
		m_Name( a_Name ), 
		m_Params( a_Params )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	const std::string & GetName() const
	{
		return m_Name;
	}
	const ParamsMap & GetParams() const 
	{
		return m_Params;
	}
	ParamsMap & GetParams()
	{
		return m_Params;
	}

	//! Mutators
	void SetName(const std::string & a_Name)
	{
		m_Name = a_Name;
	}
	void SetParams( const ParamsMap & a_Params )
	{
		m_Params = a_Params;
	}


private:
	std::string	m_Name;
	std::string m_Text;
	ParamsMap m_Params;
};

#endif
