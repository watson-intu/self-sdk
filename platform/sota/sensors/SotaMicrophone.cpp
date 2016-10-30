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

#include "SotaMicrophone.h"
#include "SelfInstance.h"
#include "utils/Log.h"
#include "utils/TimerPool.h"
#include "utils/SelfException.h"

#include <stdio.h>

#ifndef _WIN32
REG_OVERRIDE_SERIALIZABLE( Microphone, SotaMicrophone);
#endif
REG_SERIALIZABLE( SotaMicrophone );
RTTI_IMPL(SotaMicrophone, Microphone);


bool SotaMicrophone::OnStart()
{
	Log::Status("SotaMicrophone", "SotaMicrophone started");
	m_StopThread = false;
	m_ThreadStopped = false;

	ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(SotaMicrophone, ReceiveData, void *, this), NULL);
	return true;
}

bool SotaMicrophone::OnStop()
{
	// stop our streaming thread..
	Log::Status("SotaMicrophone", "SotaMicrophone stopped");
	m_StopThread = true;
	while(! m_ThreadStopped )
		tthread::this_thread::yield();
	return true;
}

void SotaMicrophone::ReceiveData(void * arg)
{
#ifndef _WIN32
	if (m_Stream != NULL)
		throw new SelfException("m_Stream is not NULL");

	while (!m_StopThread)
	{
		if (m_Paused <= 0)
		{
			if ( m_Stream == NULL )
			{
				std::string cmd = StringUtil::Format( "arecord -f S%u_LE -r %u", m_RecordingBits, m_RecordingHZ );
				Log::Debug( "SotaMicrophone", "Opening Process: %s", cmd.c_str() );
				m_Stream = popen(cmd.c_str(), "r");
			}

			char buffer[4000];		// 1/8th of a second of audio at 16,000hz

			int read = fread(buffer, sizeof(char), sizeof(buffer) / sizeof(char), m_Stream);
			if (read < 0)
				break;
			if (read > 0) 
			{
				ThreadPool::Instance()->InvokeOnMain<AudioData *>(DELEGATE(SotaMicrophone, SendingData, AudioData *, this),
					new AudioData(std::string(buffer, read), m_RecordingHZ, 1, m_RecordingBits));
			}
			else
				tthread::this_thread::sleep_for( tthread::chrono::milliseconds( 50 ) );
		}
		else
		{
			if ( m_Stream != NULL )
			{
				pclose(m_Stream);
				m_Stream = NULL;
			}

			// when paused, don't burn a bunch of CPU
			tthread::this_thread::sleep_for( tthread::chrono::milliseconds( 50 ) );
		}
	}

	if ( m_Stream != NULL )
	{
		pclose(m_Stream);
		m_Stream = NULL;
	}
#endif

	m_ThreadStopped = true;
}

void SotaMicrophone::SendingData(AudioData * a_pData)
{
	SendData(a_pData);
}

void SotaMicrophone::OnPause()
{
	m_Paused++;
}

void SotaMicrophone::OnResume()
{
	m_Paused--;
}

