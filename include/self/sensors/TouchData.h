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

#ifndef TOUCH_DATA_H
#define TOUCH_DATA_H

#include "IData.h"
#include "SelfLib.h"

class SELF_API TouchData : public IData 
{
public:
	RTTI_DECL();

	TouchData()
	{}
	TouchData(std::string a_SensorName, float a_fEngaged) : m_SensorName(a_SensorName), m_fEngaged(a_fEngaged)
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_SensorName"] = m_SensorName;
		json["m_fEngaged"] = m_fEngaged;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		m_SensorName = json["m_SensorName"].asString();
		m_fEngaged = json["m_fEngaged"].asFloat();
	}

	//!Accessors
	const std::string & GetSensorName() const
	{
		return m_SensorName;
	}

	float GetEngaged() const
	{
		return m_fEngaged;
	}

	bool IsEngaged() const
	{
		return m_fEngaged > 0.5f;
	}

private:
	//!Data
	std::string  	m_SensorName;
	float 			m_fEngaged;
};


#endif //TOUCH_DATA_H
