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

#ifndef SELF_GAZEDATA_H
#define SELF_GAZEDATA_H

#include "IData.h"
#include "SelfLib.h"

class SELF_API GazeData : public IData
{
public:
    RTTI_DECL();

    GazeData(bool a_IsLooking = false)
            : m_IsLooking(a_IsLooking)
    {}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_IsLooking"] = m_IsLooking;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_IsLooking = json["m_IsLooking"].asBool();
	}

    //!Accessors
    bool GetIsLooking() const
    {
        return m_IsLooking;
    }
private:

    //!Data
    bool			m_IsLooking;
};

#endif //SELF_GAZEDATA_H
