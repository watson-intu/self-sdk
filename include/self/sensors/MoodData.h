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

#ifndef SELF_MOODDATA_H
#define SELF_MOODDATA_H

#include "SelfLib.h"

class SELF_API MoodData : public IData
{
public:
    RTTI_DECL();

	//! Construction
    MoodData(bool a_IsSmiling = false)
            : m_IsSmiling(a_IsSmiling)
    {}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_IsSmiling"] = m_IsSmiling;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_IsSmiling = json["m_IsSmiling"].asBool();
	}
	
	//!Accessors
    bool GetIsSmiling() const
    {
        return m_IsSmiling;
    }

private:
    //!Data
    bool			m_IsSmiling;
};

#endif //SELF_MOODDATA_H
