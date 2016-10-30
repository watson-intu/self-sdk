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

#include "LookupIntent.h"

REG_SERIALIZABLE(LookupIntent);
RTTI_IMPL(LookupIntent, IIntent);

void LookupIntent::Serialize(Json::Value &json)
{
	IIntent::Serialize(json);

	json["m_Text"] = m_Text;
	json["m_Collection"] = m_Collection;
	json["m_Entity"] = m_Entity;
	json["m_Intent"] = m_Intent;
}

void LookupIntent::Deserialize(const Json::Value &json)
{
	IIntent::Deserialize(json);

	m_Text = json["m_Text"].asString();
	m_Collection = json["m_Collection"].asString();
	m_Entity = json["m_Entity"].asString();
	m_Intent = json["m_Intent"].asString();
}

void LookupIntent::Create(const Json::Value & a_Intent, const Json::Value & a_Parse)
{
	if (a_Intent.isMember("text"))
		m_Text = a_Intent["text"].asString();
	if (a_Intent.isMember("top_class"))
		m_Intent = a_Intent["top_class"].asString();
	if (a_Intent.isMember("conversation"))
	{
		m_Entity = a_Intent["conversation"]["entities"][0]["entity"].asString();
		m_Collection = a_Intent["goal_params"]["answer"]["response"][0].asString();
	}
}

