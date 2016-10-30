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

#ifndef SELF_REMOTEDEVICEEXTRACTOR_H
#define SELF_REMOTEDEVICEEXTRACTOR_H

#include "IFeatureExtractor.h"
#include "sensors/SensorManager.h"
#include "sensors/RemoteDeviceData.h"
#include "blackboard/Environment.h"
#include "utils/Factory.h"
#include "SelfLib.h"

class SelfInstance;

class SELF_API RemoteDeviceExtractor : public IFeatureExtractor
{
public:
    RTTI_DECL();

    RemoteDeviceExtractor() {}


    //! IFeatureExtractor interface
    virtual const char * GetName() const;
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    typedef SensorManager::SensorList	SensorList;

    //! Data
    SensorList		                    m_RemoteDeviceSensors;

    //! Callback handler
    void                                OnRemoteDeviceData(IData * data);
};

#endif
