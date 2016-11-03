/**
* Copyright 2016 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef SELF_MOTIONEXTRACTOR_H
#define SELF_MOTIONEXTRACTOR_H

#include "extractors/IFeatureExtractor.h"
#include "sensors/SensorManager.h"
#include "sensors/RemoteDeviceData.h"
#include "blackboard/Motion.h"
#include "utils/Factory.h"
#include "SelfLib.h"

class SelfInstance;

class MotionExtractor : public IFeatureExtractor
{
public:
	RTTI_DECL();

	MotionExtractor() {}


	//! IFeatureExtractor interface
	virtual const char * GetName() const;
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Types
	typedef SensorManager::SensorList	SensorList;

	//! Data
	SensorList		                    m_MotionSensors;

	//! Callback handler
	void                                OnMotionData(IData * data);
};

#endif

