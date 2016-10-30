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

#ifndef SELF_MATHINTENT_H
#define SELF_MATHINTENT_H

#include "blackboard/IIntent.h"

class MathIntent : public IIntent
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<MathIntent>		SP;
	typedef boost::weak_ptr<MathIntent>			WP;

	//! Construction
	MathIntent()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IIntent interface
	virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

	const std::string & GetOperation() const
	{
		return m_Operation;
	}

private:
	//! Data
	std::string m_Operation;
};

#endif //SELF_QUESTIONINTENT_H
