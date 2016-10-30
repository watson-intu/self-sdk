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

#ifndef HEALTH_DATA_H
#define HEALTH_DATA_H

#include "jsoncpp/json/json.h"

#include "IData.h"
#include "SelfLib.h"

class SELF_API HealthData : public IData
{
public:
    RTTI_DECL();

	//! Construction
	HealthData()
	{}
	HealthData( const std::string & a_HealthName, const Json::Value & a_Content ) :
		m_HealthName( a_HealthName ), m_Content( a_Content )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_HealthName"] = m_HealthName;
		json["m_Content"] = m_Content;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_HealthName = json["m_HealthName"].asString();
		m_Content = json["m_Content"];
	}

    //!Accessors
    const std::string & GetHealthName() const
    {
        return m_HealthName;
    }

	Json::Value & GetContent()
	{
		return m_Content;
	}

	//!Mutators
	void SetHealthName( std::string & a_HealthName )
	{
		m_HealthName = a_HealthName;
	}

private:
    //!Data
    std::string     m_HealthName;  // name of service, joint, etc
	Json::Value     m_Content;
};


#endif //HealthData
