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

#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "MotionExtractor.h"

REG_SERIALIZABLE(MotionExtractor);
RTTI_IMPL(MotionExtractor, IFeatureExtractor);

const char * MotionExtractor::GetName() const
{
	return "MotionExtractor";
}

bool MotionExtractor::OnStart()
{
	// find all remote device sensors, then subscribe to them to receive their input..
	SelfInstance::GetInstance()->GetSensorManager()->FindSensors(RemoteDeviceData::GetStaticRTTI(), m_MotionSensors);
	for (SensorManager::SensorList::iterator iSensor = m_MotionSensors.begin(); iSensor != m_MotionSensors.end(); ++iSensor)
	{
		(*iSensor)->Subscribe(DELEGATE(MotionExtractor, OnMotionData, IData *, this));
	}

	Log::Status("MotionExtractor", "MotionExtractor started");
	return true;
}

bool MotionExtractor::OnStop()
{
	for (SensorManager::SensorList::iterator iSensor = m_MotionSensors.begin(); iSensor != m_MotionSensors.end(); ++iSensor)
		(*iSensor)->Unsubscribe(this);
	m_MotionSensors.clear();

	Log::Status("MotionExtractor", "MotionExtractor stopped");
	return true;
}

void MotionExtractor::OnMotionData(IData * data)
{
	// TODO: Needs to make sense if it is an image, text, or environment - defaulting to environment now
	// This is going to take in "data" from the sensor and add a live motion object to it.
	RemoteDeviceData * pRemoteDevice = DynamicCast<RemoteDeviceData>(data);
	if (pRemoteDevice != NULL)
	{
		Motion::SP spMotion(new Motion());
		if (spMotion->Create(pRemoteDevice->GetContent()))
			SelfInstance::GetInstance()->GetBlackBoard()->AddThing(spMotion);
	}
}



