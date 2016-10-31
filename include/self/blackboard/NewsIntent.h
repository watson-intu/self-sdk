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

#ifndef SELF_NEWSINTENT_H
#define SELF_NEWSINTENT_H

#include "IIntent.h"

class SELF_API NewsIntent : public IIntent
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<NewsIntent>		SP;
	typedef boost::weak_ptr<NewsIntent>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IIntent interface
	virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

	const std::string & GetCompany() const
	{
		return m_Company;
	}

private:
	//! Data
	std::string m_Company;
	Json::Value m_Parse;
};

#endif //SELF_WEATHERINTENT_H