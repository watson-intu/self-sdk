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

#include "AQMicrophone.h"

REG_SERIALIZABLE(AQMicrophone);
RTTI_IMPL(AQMicrophone, Microphone);

#ifndef _WIN32

#include "portaudio.h"
REG_OVERRIDE_SERIALIZABLE(Microphone, AQMicrophone);

static int OnCapturedAudio(const void * a_InputBuffer, void * a_OutputBuffer,
	unsigned long a_FramesPerBuffer, const PaStreamCallbackTimeInfo * a_Timer,
	PaStreamCallbackFlags a_Status, void * a_UserData)
{
	AQMicrophone * p_Mic((AQMicrophone *)a_UserData);
	ThreadPool::Instance()->InvokeOnMain<AudioData *>(DELEGATE(AQMicrophone, SendingData, AudioData *, p_Mic),
		new AudioData(std::string((const char *)a_InputBuffer, a_FramesPerBuffer * 2), 16000, 1, 16));
	return 0;
}

#endif

bool AQMicrophone::OnStart()
{
	Log::Debug("AQMicrophone", "Starting AQ Microphone!");
	m_StopThread = false;
	m_ThreadStopped = false;

	ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(AQMicrophone, CaptureAudio, void *, this), 0);
	return true;
}

bool AQMicrophone::OnStop()
{
	Log::Debug("AQMicrophone", "Stopping AQMicrophone!");
	m_StopThread = true;
	while (!m_ThreadStopped)
		tthread::this_thread::yield();
	return true;
}

void AQMicrophone::CaptureAudio(void * arg)
{
#ifndef _WIN32
	Log::Debug("AQMicrophone", "Beginning to capture audio!");
	PaStreamParameters inputParameters;
	PaStream * stream;
	PaError     err = paNoError;

	const PaDeviceInfo * deviceInfo;
	int defaultDisplayed;

	err = Pa_Initialize();
	if (err != paNoError)
	{
		Log::Error("AQMicrophone", "Error initializing PortAudio Stream");
		m_ThreadStopped = true;
		m_StopThread = true;
		return;
	}

	inputParameters.device = Pa_GetDefaultInputDevice();
	if (inputParameters.device == paNoDevice)
	{
		Log::Error("AQMicrophone", "No default input device recognized for Port Audio");
	}
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = paInt16;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	Pa_OpenStream(&stream,
		&inputParameters,
		NULL,
		m_RecordingHZ,
		4000,
		paClipOff,
		OnCapturedAudio,
		this);

	Pa_StartStream(stream);

	while (!m_StopThread)
	{
		tthread::this_thread::sleep_for(tthread::chrono::milliseconds(50));
	}
	Pa_CloseStream(stream);
#endif
	m_ThreadStopped = true;
}

void AQMicrophone::OnPause()
{
	m_Paused++;
}

void AQMicrophone::OnResume()
{
	m_Paused--;
}

void AQMicrophone::SendingData(AudioData * a_pData)
{
	if (m_Paused <= 0)
		SendData(a_pData);
}
