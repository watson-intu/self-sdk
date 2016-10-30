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

#ifndef SELF_CAMERA_H
#define SELF_CAMERA_H

#include "SelfInstance.h"
#include "ISensor.h"
#include "VideoData.h"

#include "utils/ThreadPool.h"
#include "utils/Time.h"

#include "SelfLib.h"

//! Base class for a Camera sensor class that collects VideoData. This is not a actual implementation, see NaoCamera.
class SELF_API Camera : public ISensor
{
public:
	RTTI_DECL();

	Camera() : m_fFramesPerSec( 1.0f )
	{}

	//! ISerialiazable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "Camera";
	}
	virtual const char * GetDataType()
	{
		return VideoData::GetStaticRTTI().GetName().c_str();
	}
	virtual const char * GetBinaryType()
	{
		return "image/jpeg";
	}

	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

protected:
	//! Data
	float		m_fFramesPerSec;
};

#endif	// SELF_CAMERA_H