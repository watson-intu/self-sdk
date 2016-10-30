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

#ifndef WDC_ICONDITIONAL_H
#define WDC_ICONDITIONAL_H

#include "utils/Logic.h"
#include "utils/JsonHelpers.h"
#include "utils/RTTI.h"
#include "utils/ISerializable.h"

#include "WDCLib.h"

//! Base class for an object for testing conditions.
struct WDC_API IConditional : public ISerializable
{
	RTTI_DECL();

	IConditional()
	{}

	//! Types
	typedef boost::shared_ptr<IConditional>		SP;
	typedef boost::weak_ptr<IConditional>		WP;

	virtual IConditional * Clone() const = 0;
	virtual bool Test(const Json::Value & a_Test) = 0;
};

//! A null condition object, when no condition testing is needed but you still need to pass an object by reference.
struct WDC_API NullCondition : public IConditional
{
	RTTI_DECL();

	NullCondition()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{}
	virtual void Deserialize(const Json::Value & json)
	{}
	//! IConditional interface
	virtual IConditional * Clone() const
	{
		return new NullCondition( *this );
	}
	virtual bool Test(const Json::Value & a_Test)
	{
		return true;
	}
};

//! Compare the value at m_Path to m_Value with the provided
//! equality operator. 
struct WDC_API EqualityCondition : public IConditional
{
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<EqualityCondition>		SP;
	typedef boost::weak_ptr<EqualityCondition>			WP;

	EqualityCondition()
	{}
	EqualityCondition(const std::string & a_Path, Logic::EqualityOp a_Op, const Json::Value & a_Value) :
		m_Path(a_Path),
		m_EqualOp(a_Op),
		m_Value(a_Value)
	{}

	std::string				m_Path;
	Logic::EqualityOp		m_EqualOp;
	Json::Value				m_Value;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_Path"] = m_Path;
		json["m_EqualOp"] = Logic::EqualityOpText(m_EqualOp);
		json["m_Value"] = m_Value;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_Path = json["m_Path"].asString();
		m_EqualOp = Logic::GetEqualityOp(json["m_EqualOp"].asString());
		m_Value = json["m_Value"];
	}

	//! IConditional interface
	virtual IConditional * Clone() const
	{
		return new EqualityCondition( *this );
	}
	virtual bool Test(const Json::Value & a_Test)
	{
		return Logic::TestEqualityOp(m_EqualOp, JsonHelpers::Resolve(a_Test, m_Path), m_Value);
	}
};

//! Compares the collection of conditions, then applies the logical operation (AND,OR,XOR)
struct WDC_API LogicalCondition : public IConditional
{
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<LogicalCondition>		SP;
	typedef boost::weak_ptr<LogicalCondition>		WP;
	typedef std::list<IConditional::SP>				Conditions;

	LogicalCondition()
	{}
	LogicalCondition(Logic::LogicalOp a_Op, 
		const Conditions & a_Conditions) : m_LogicOp(a_Op), m_Conditions(a_Conditions)
	{}
	LogicalCondition(Logic::LogicalOp a_Op, 
		const IConditional & a_Cond1 ) : m_LogicOp(a_Op)
	{
		m_Conditions.push_back(IConditional::SP(a_Cond1.Clone()));
	}
	LogicalCondition(Logic::LogicalOp a_Op,
		const IConditional & a_Cond1,
		const IConditional & a_Cond2 ) : m_LogicOp(a_Op)
	{
		m_Conditions.push_back(IConditional::SP(a_Cond1.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond2.Clone()));
	}
	LogicalCondition(Logic::LogicalOp a_Op, 
		const IConditional & a_Cond1, 
		const IConditional & a_Cond2,
		const IConditional & a_Cond3 ) : m_LogicOp(a_Op)
	{
		m_Conditions.push_back(IConditional::SP(a_Cond1.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond2.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond3.Clone()));
	}
	LogicalCondition(Logic::LogicalOp a_Op,
		const IConditional & a_Cond1,
		const IConditional & a_Cond2,
		const IConditional & a_Cond3,
		const IConditional & a_Cond4 ) : m_LogicOp(a_Op)
	{
		m_Conditions.push_back(IConditional::SP(a_Cond1.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond2.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond3.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond4.Clone()));
	}
	LogicalCondition(Logic::LogicalOp a_Op,
		const IConditional & a_Cond1,
		const IConditional & a_Cond2,
		const IConditional & a_Cond3,
		const IConditional & a_Cond4,
		const IConditional & a_Cond5 ) : m_LogicOp(a_Op)
	{
		m_Conditions.push_back(IConditional::SP(a_Cond1.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond2.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond3.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond4.Clone()));
		m_Conditions.push_back(IConditional::SP(a_Cond5.Clone()));
	}

	Logic::LogicalOp		m_LogicOp;
	Conditions				m_Conditions;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_LogicOp"] = Logic::LogicalOpText(m_LogicOp);
		SerializeList("m_Conditions", m_Conditions, json);
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_LogicOp = Logic::GetLogicalOp(json["m_LogicOp"].asString());
		DeserializeList("m_Conditions", json, m_Conditions);
	}

	//! IConditional interface
	virtual IConditional * Clone() const
	{
		return new LogicalCondition( *this );
	}
	virtual bool Test(const Json::Value & a_Test)
	{
		std::vector<bool> conds;
		for (Conditions::const_iterator iCondition = m_Conditions.begin(); iCondition != m_Conditions.end(); ++iCondition)
			conds.push_back((*iCondition)->Test(a_Test));

		return Logic::TestLogicalOp(m_LogicOp, conds);
	}
};

#endif // SELF_DATA_STORE_H
