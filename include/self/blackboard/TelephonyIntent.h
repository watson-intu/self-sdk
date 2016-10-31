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

#ifndef SELF_TELEPHONYINTENT_H
#define SELF_TELEPHONYINTENT_H

#include "IIntent.h"

class SELF_API TelephonyIntent : public IIntent
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<TelephonyIntent>		    SP;
	typedef boost::weak_ptr<TelephonyIntent>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IIntent interface
	virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

	void SetText(const std::string & a_Text)
	{
		m_Text = a_Text;
	}

	const std::string & GetText() const
	{
		return m_Text;
	}

private:
	//! Data
	std::string m_Text;
};

#endif //SELF_TELEPHONYINTENT_H