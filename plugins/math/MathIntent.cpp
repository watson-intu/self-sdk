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

#include "MathIntent.h"

REG_SERIALIZABLE(MathIntent);
RTTI_IMPL( MathIntent, IIntent );

void MathIntent::Serialize(Json::Value &json)
{
	IIntent::Serialize(json);

	json["m_Operation"] = m_Operation;
}

void MathIntent::Deserialize(const Json::Value &json)
{
	IIntent::Deserialize(json);

	m_Operation = json["m_Operation"].asString();
}

void MathIntent::Create(const Json::Value & a_Intent, const Json::Value & a_Parse)
{
	if ( a_Intent.isMember("text") )
		m_Operation = a_Intent["text"].asString();
	m_Confidence = 1.0f;
}

