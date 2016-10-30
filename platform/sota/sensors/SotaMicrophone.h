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

#ifndef SOTA_MICROPHONE_H
#define SOTA_MICROPHONE_H

#include "sensors/Microphone.h"

//! This ISensor gets audio data from the Nao microphone input.
class SotaMicrophone : public Microphone
{
public:
	RTTI_DECL();

	//! Construction
	SotaMicrophone() : m_Stream(NULL), m_StopThread( false ), m_ThreadStopped( false ), m_Paused( 0 )
	{}

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "Microphone";
	}

	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

private:
	//! Data
	FILE *				m_Stream;
	volatile bool		m_StopThread;
	volatile int 		m_Paused;
	volatile bool		m_ThreadStopped;

	void				ReceiveData( void * );
	void				SendingData( AudioData * a_pData );
};

#endif
