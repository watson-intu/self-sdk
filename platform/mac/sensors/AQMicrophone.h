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

#ifndef SELF_AQMICROPHONE_H
#define SELF_AQMICROPHONE_H

#include "sensors/Microphone.h"
#include "SelfInstance.h"
#include "utils/TimerPool.h"

//! Audio Queue Microphone
class AQMicrophone : public Microphone
{
public:
	RTTI_DECL();

	//! Construction
	AQMicrophone() : m_StopThread(false), m_ThreadStopped(false), m_Paused(0)
	{}

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "Microphone";
	}

	//! ISensor interface
	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

	void				SendingData(AudioData * a_pData);

private:

	//! Data
	volatile bool		m_StopThread;
	volatile bool		m_ThreadStopped;
	volatile int 		m_Paused;

	void                CaptureAudio(void * arg);
};

#endif //SELF_MACMICROPHONE_H
