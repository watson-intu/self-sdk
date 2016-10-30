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

#ifndef SELF_GOAL_PARAMS_CONDTION_H
#define SELF_GOAL_PARAMS_CONDTION_H

#include "planning/ICondition.h"

//! This condition tests against one or more of the params of the Goal object.
struct SELF_API GoalParamsCondition : public ICondition
{
	RTTI_DECL();

	//! Types
	struct ParamCondition
	{
		ParamCondition() : m_Op(EQ)
		{}
		ParamCondition(const std::string & a_Name, const Json::Value & a_Value, EqualityOp a_Op) :
			m_Name(a_Name),
			m_Op(a_Op),
			m_Value(a_Value)
		{}

		std::string		m_Name;
		Json::Value		m_Value;
		EqualityOp		m_Op;
	};

	//! COnstructions
	GoalParamsCondition() : m_LogicalOp(AND)
	{}
	GoalParamsCondition(const std::vector<ParamCondition> & a_Params, LogicalOp a_LogOp = AND) :
		m_Params(a_Params), m_LogicalOp(a_LogOp)
	{}
	GoalParamsCondition(const std::string & a_Name, const  Json::Value & a_Value, 
		EqualityOp a_Op = ICondition::EQ, LogicalOp a_LogOp = AND) : m_LogicalOp(a_LogOp)
	{
		m_Params.push_back(ParamCondition(a_Name, a_Value, a_Op));
	}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ICondition interface
	virtual float Test(Goal::SP a_spGoal);
	virtual ICondition * Clone();

	std::vector<ParamCondition>
						m_Params;
	LogicalOp			m_LogicalOp;
};

#endif
