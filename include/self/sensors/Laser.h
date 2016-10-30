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

#ifndef SELF_LASER_H
#define SELF_LASER_H

#include "SelfInstance.h"
#include "ISensor.h"
#include "LaserData.h"

#include "utils/ThreadPool.h"
#include "utils/Time.h"

#include "SelfLib.h"

//! Base class for a Laser Sensor class
class SELF_API Laser : public ISensor
{
public:
    RTTI_DECL();

    Laser()
    {}

    //! ISerialiazable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "Laser";
    }
    virtual const char * GetDataType()
    {
        return LaserData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

};

#endif	// SELF_LASER_H