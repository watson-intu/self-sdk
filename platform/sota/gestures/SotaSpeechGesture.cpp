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


#include "SotaSpeechGesture.h"
#include "sensors/AudioData.h"
#include "services/TextToSpeech/TextToSpeech.h"
#include "skills/SkillManager.h"
#include "utils/ThreadPool.h"
#include "utils/Sound.h"

#include "SelfInstance.h"

REG_OVERRIDE_SERIALIZABLE(SpeechGesture, SotaSpeechGesture);
REG_SERIALIZABLE(SotaSpeechGesture);
RTTI_IMPL(SotaSpeechGesture, SpeechGesture);

bool SotaSpeechGesture::Start()
{
	if (!SpeechGesture::Start())
		return false;

	m_pTTS = SelfInstance::GetInstance()->FindService<TextToSpeech>();
	if (m_pTTS == NULL)
	{
		Log::Error("DSSPeechGesture", "TextToSpeech service is missing.");
		return false;
	}

	m_pTTS->GetVoices(DELEGATE(SotaSpeechGesture, OnVoices, Voices *, this));
	return true;
}

bool SotaSpeechGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
	if (PushRequest(a_Callback, a_Params))
		StartSpeech();

	return true;
}

void SotaSpeechGesture::StartSpeech()
{
	if (m_pVoices != NULL)
	{
		bool bSuccess = false;

		Request * pReq = ActiveRequest();

		const std::string & text = pReq->m_Params["text"].asString();
		const std::string & gender = pReq->m_Params["gender"].asString();
		const std::string & language = pReq->m_Params["language"].asString();

		std::string voice = "en-US_MichaelVoice";
		for (size_t i = 0; i < m_pVoices->m_Voices.size(); ++i)
		{
			Voice & v = m_pVoices->m_Voices[i];
			if (v.m_Language == language && v.m_Gender == gender)
			{
				voice = v.m_Name;
				break;
			}
		}

		TextToSpeech * pTTS = SelfInstance::GetInstance()->FindService<TextToSpeech>();
		if (pTTS != NULL)
		{
			// call the service to get the audio data for playing ..
			pTTS->SetVoice(voice);
			pTTS->ToSound(text, DELEGATE(SotaSpeechGesture, OnSpeechData, Sound *, this));
			bSuccess = true;
		}
		else
			Log::Error("DSSpeechGesture", "No TextToSpeech service available.");

		if (!bSuccess)
			OnSpeechDone();
	}

}

bool SotaSpeechGesture::Abort()
{
	return false;
}

void SotaSpeechGesture::OnVoices(Voices * a_pVoices)
{
	m_pVoices = a_pVoices;
	if (ActiveRequest() != NULL)
		StartSpeech();
}

void SotaSpeechGesture::OnSpeechData(Sound * a_pSound)
{
	Request * pReq = ActiveRequest();

	// play the provided WAV file..
	if (a_pSound != NULL)
	{
		ThreadPool::Instance()->InvokeOnThread<Sound *>(DELEGATE(SotaSpeechGesture, OnPlaySpeech, Sound *, this), a_pSound);
	}
	else
	{
		Log::Error("SotaSpeechGesture", "Failed to load WAV data.");
		OnSpeechDone();
	}
}

void SotaSpeechGesture::OnPlaySpeech(Sound * a_pSound)
{
#ifndef _WIN32
	std::string wav_file_name = "WatsonTTS4Sota.wav";
	if (a_pSound->SaveToFile("tmp.wav"))
	{
		FILE * stream = popen("aplay -q tmp.wav", "r");
		char buffer[1024];
		int read = fread(buffer, sizeof(char), sizeof(buffer) / sizeof(char), stream);
		fclose(stream);
	}
#endif

	ThreadPool::Instance()->InvokeOnMain(VOID_DELEGATE(SotaSpeechGesture, OnSpeechDone, this));
}

void SotaSpeechGesture::OnSpeechDone()
{
	//SelfInstance::GetInstance()->GetSensorManager()->ResumeSensorType( "AudioData" );

	// start the next speech if we have any..
	if (PopRequest())
		StartSpeech();
}
