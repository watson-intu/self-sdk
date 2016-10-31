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


#ifndef SELF_CALCULATE_H
#define SELF_CALCULATE_H

#include "IThing.h"

//! This object is placed on the blackboard when we need self to calculate something
class SELF_API Calculate : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Calculate>			SP;
	typedef boost::weak_ptr<Calculate>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Construction
	Calculate() : IThing(TT_COGNITIVE)
	{}

	Calculate(const std::string & a_Data,
		const std::string & a_Arithmetic,
		const std::string & a_Key) :
		IThing(TT_COGNITIVE),
		m_Data(a_Data),
		m_Arithmetic(a_Arithmetic),
		m_Key(a_Key)
	{}

	//! Accessors
	const std::string & GetData() const
	{
		return m_Data;
	}

	const std::string & GetArithmetic() const
	{
		return m_Arithmetic;
	}

	const std::string & GetKey() const
	{
		return m_Key;
	}

private:
	//! Data
	std::string m_Data;
	std::string m_Arithmetic;
	std::string m_Key;
};

#endif //SELF_CALCULATE_H
