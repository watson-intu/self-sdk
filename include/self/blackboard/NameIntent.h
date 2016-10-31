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

#ifndef SELF_NAMEINTENT_H
#define SELF_NAMEINTENT_H

#include "IIntent.h"

class SELF_API NameIntent : public IIntent
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<NameIntent>		SP;
	typedef boost::weak_ptr<NameIntent>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IIntent interface
	virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

	const std::string & GetName() const
	{
		return m_Name;
	}

	void SetName(const std::string & a_Name)
	{
		m_Name = a_Name;
	}
	
	const std::string & GetPossesive() const
	{
		return m_Possessive;
	}

private:
	//! Data
	std::string m_Name;
	std::string m_Possessive;
	Json::Value m_Parse;
};

#endif //SELF_NAMEINTENT_H