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

//! This condition contains a list of conditions, it can apply a logical AND/OR to the conditions,
//! it also has a NOT that can be applied at the end to flip the true/false state.
struct SELF_API ConditionList : public ICondition
{
	RTTI_DECL();

	//! Construction
	ConditionList() : m_LogicalOp(AND), m_NOT( false ), m_fMinTrue( 0.5f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ICondition interface
	virtual float Test(Goal::SP a_spGoal);
	virtual ICondition * Clone();

	std::vector<ICondition::SP>	m_Conditions;		// additional conditions to test
	LogicalOp					m_LogicalOp;		// logical operation between all conditions
	bool						m_NOT;				// if true, then Test is inverted
	float						m_fMinTrue;			// what is the minimum value of true
};

#endif
