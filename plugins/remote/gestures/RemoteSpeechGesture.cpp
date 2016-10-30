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


#include "RemoteSpeechGesture.h"
#include "services/TextToSpeech/TextToSpeech.h"
#include "skills/SkillManager.h"
#include "utils/ThreadPool.h"
#include "utils/Sound.h"

#include "SelfInstance.h"

REG_OVERRIDE_SERIALIZABLE(SpeechGesture, RemoteSpeechGesture);
REG_SERIALIZABLE(RemoteSpeechGesture);
RTTI_IMPL( RemoteSpeechGesture, SpeechGesture );

bool RemoteSpeechGesture::Start()
{
	if (! SpeechGesture::Start() )
		return false;

	m_pTTS = SelfInstance::GetInstance()->FindService<TextToSpeech>();
	if ( m_pTTS == NULL )
	{
		Log::Error( "DSSPeechGesture", "TextToSpeech service is missing." );
		return false;
	}

	ITopics * pTopics = SelfInstance::GetInstance()->GetTopics();
	pTopics->RegisterTopic( "audio-out", "audio/L16;rate=22050" );
	
	m_pTTS->GetVoices( DELEGATE( RemoteSpeechGesture, OnVoices, Voices *, this ) );
	return true;
}

bool RemoteSpeechGesture::Stop()
{
	if (! SpeechGesture::Stop() )
		return false;

	ITopics * pTopics = SelfInstance::GetInstance()->GetTopics();
	pTopics->UnregisterTopic( "audio-out" );

	return true;
}

bool RemoteSpeechGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
	if ( PushRequest( a_Callback, a_Params ) )
		StartSpeech();

	return true;
}

void RemoteSpeechGesture::StartSpeech()
{
	if ( m_pVoices != NULL )
	{
		bool bSuccess = false;

		ITopics * pTopics = SelfInstance::GetInstance()->GetTopics();
		if ( pTopics->GetSubscriberCount( "audio-out" ) > 0 )
		{
			Request * pReq = ActiveRequest();

			const std::string & text = pReq->m_Params["text"].asString();
			const std::string & gender = pReq->m_Params["gender"].asString();
			const std::string & language = pReq->m_Params["language"].asString();

			std::string voice = "en-US_MichaelVoice";
			for(size_t i=0;i<m_pVoices->m_Voices.size();++i)
			{
				Voice & v = m_pVoices->m_Voices[i];
				if ( v.m_Language == language && v.m_Gender == gender )
				{
					voice = v.m_Name;
					break;
				}
			}

			TextToSpeech * pTTS = SelfInstance::GetInstance()->FindService<TextToSpeech>();
			if ( pTTS != NULL )
			{
				// call the service to get the audio data for playing ..
				pTTS->SetVoice(voice);
				pTTS->ToSound( text, DELEGATE( RemoteSpeechGesture, OnSpeechData, Sound *, this ) );
				bSuccess = true;
			}
			else
				Log::Error( "RemoteSpeechGesture", "No TextToSpeech service available." );
		}

		if (! bSuccess )
			OnSpeechDone();
	}
}

bool RemoteSpeechGesture::Abort()
{
	return false;
}

void RemoteSpeechGesture::OnVoices( Voices * a_pVoices )
{
	m_pVoices = a_pVoices;
	if ( ActiveRequest() != NULL )
		StartSpeech();
}

void RemoteSpeechGesture::OnSpeechData( Sound * a_Data )
{
	if ( a_Data != NULL )
	{
		ITopics * pTopics = SelfInstance::GetInstance()->GetTopics();
		pTopics->Publish( "audio-out", a_Data->GetWaveData(), false, true );
		delete a_Data;
	}

	OnSpeechDone();
}

void RemoteSpeechGesture::OnSpeechDone()
{
	// start the next speech if we have any..
	if ( PopRequest() )
		StartSpeech();
}

