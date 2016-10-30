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

#ifndef SELF_GESTUREDATA_H
#define SELF_GESTUREDATA_H

#include "IData.h"
#include "SelfLib.h"

class SELF_API GestureData : public IData
{
public:
    RTTI_DECL();

	GestureData()
	{}
    GestureData(const std::string & a_Gesture)
            : m_Gesture(a_Gesture)
    {}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_Gesture"] = m_Gesture;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_Gesture = json["m_Gesture"].asString();
	}

	//!Accessors
    const std::string & GetGesture() const
    {
        return m_Gesture;
    }

    void SetGesture(const std::string & a_Gesture)
    {
        m_Gesture = a_Gesture;
    }

private:

    //!Data
    std::string	m_Gesture;
};

#endif //SELF_GESTUREDATA_H
