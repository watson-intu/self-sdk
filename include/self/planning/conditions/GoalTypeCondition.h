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

#ifndef SELF_GOAL_TYPE_CONDTION_H
#define SELF_GOAL_TYPE_CONDTION_H

#include "planning/ICondition.h"

//! This condition tests against the type of goal
struct SELF_API GoalTypeCondition : public ICondition
{
	RTTI_DECL();

	GoalTypeCondition() : m_GoalTypeOp(EQ)
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ICondition interface
	virtual float Test(Goal::SP a_spGoal);
	virtual ICondition * Clone();

	std::string			m_GoalType;
	EqualityOp			m_GoalTypeOp;
};

#endif
