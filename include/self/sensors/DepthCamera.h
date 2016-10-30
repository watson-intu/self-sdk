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

#ifndef SELF_DEPTHCAMERA_H
#define SELF_DEPTHCAMERA_H

#include "SelfInstance.h"
#include "ISensor.h"
#include "DepthVideoData.h"

#include "utils/ThreadPool.h"
#include "utils/Time.h"

#include "SelfLib.h"

//! Base class for a 3DCamera sensor class that collects VideoData. This is not a actual implementation, see NaoCamera.
class SELF_API DepthCamera : public ISensor
{
public:
    RTTI_DECL();

    DepthCamera() : m_fFramesPerSec( 1.0f )
    {}

    //! ISerialiazable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "DepthCamera";
    }
	virtual const char * GetDataType()
	{
		return DepthVideoData::GetStaticRTTI().GetName().c_str();
	}

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

protected:
    //! Data
    float		m_fFramesPerSec;
};

#endif //SELF_DEPTHCAMERA_H
