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

#ifndef SELF_GESTURESENSOR_H
#define SELF_GESTURESENSOR_H

#include "SelfInstance.h"
#include "ISensor.h"
#include "GestureData.h"

#include "utils/ThreadPool.h"
#include "utils/Time.h"

#include "SelfLib.h"

//! Base class for a Gesture Sensor class
class SELF_API GestureSensor : public ISensor
{
public:
    RTTI_DECL();

    GestureSensor()
    {}

    //! ISerialiazable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "GestureSensor";
    }
    virtual const char * GetDataType()
    {
        return GestureData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

};

#endif	// SELF_GESTURESENSOR_H