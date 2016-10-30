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

#ifndef SELF_HW_DEVICE_H
#define SELF_HW_DEVICE_H

#include "SelfInstance.h"
#include "ISensor.h"
#include "HealthData.h"

#include "utils/TimerPool.h"
#include "utils/Time.h"

#include "SelfLib.h"

//! Base class for a Hardware Device sensor class
class SELF_API HealthSensor : public ISensor
{
public:
    RTTI_DECL();

    HealthSensor() : m_fLowBatteryThreshold( 10.0f )
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "Health";
    }
    virtual const char * GetDataType()
    {
        return HealthData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

protected:
    //! Data
    std::vector<std::string>	m_SensorReadings;
    std::vector<std::string>	m_ErrorDiagnosis;
    int                         m_HealthSensorCheckInterval;
	float                       m_fLowBatteryThreshold;
};

#endif	// SELF_HW_DEVICE_H