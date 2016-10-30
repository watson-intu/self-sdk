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

#ifndef SELF_TEXT_DATA_H
#define SELF_TEXT_DATA_H

#include "IData.h"
#include "SelfLib.h"

//! This object wraps a simple string in the IData interface. This carries input from keyboards or local STT devices.
class SELF_API TextData : public IData 
{
public:
	RTTI_DECL();

	TextData()
	{}
	TextData(const std::string & a_Text, float a_fConfidence) 
		: m_Text( a_Text ), m_fConfidence( a_fConfidence )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_Text"] = m_Text;
		json["m_fConfidence"] = m_fConfidence;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_Text = json["m_Text"].asString();
		m_fConfidence = json["m_fConfidence"].asFloat();
	}

	//!Accessors
	const std::string & GetText() const
	{
		return m_Text;
	}
	float GetConfidence() const
	{
		return m_fConfidence;
	}
private:
	//!Data
	std::string  	m_Text;
	float			m_fConfidence;
};


#endif //TouchData
