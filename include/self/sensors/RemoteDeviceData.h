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

#ifndef SELF_REMOTEDEVICEDATA_H
#define SELF_REMOTEDEVICEDATA_H

#include "jsoncpp/json/json.h"

#include "IData.h"
#include "SelfLib.h"

//! This data type contains raw wave audio data..
class SELF_API RemoteDeviceData : public IData
{
public:
    RTTI_DECL();

	//! Construction
	RemoteDeviceData()
	{}
    RemoteDeviceData(const Json::Value & a_Content) : m_Content( a_Content )
    {}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_Content"] = m_Content;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_Content = json["m_Content"];
	}
	
	//!Accessors
    const Json::Value & GetContent() const
    {
        return m_Content;
    }

private:
    //!Data
    Json::Value  	    m_Content;
};

#endif
