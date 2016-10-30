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


#include "DSSpeechGesture.h"
#include "sensors/AudioData.h"
#include "services/TextToSpeech/TextToSpeech.h"
#include "skills/SkillManager.h"
#include "utils/ThreadPool.h"
#include "utils/Sound.h"

#include "SelfInstance.h"

#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#include <dxerr.h>

// HACK: to make an older dxerr.lib work with VS 2015
#pragma warning(disable:4996)
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
#pragma comment( lib, "dxerr.lib" )

LPDIRECTSOUND8  DSSpeechGesture::sm_pDirectSound = NULL;

REG_OVERRIDE_SERIALIZABLE(SpeechGesture, DSSpeechGesture);
REG_SERIALIZABLE(DSSpeechGesture);
RTTI_IMPL( DSSpeechGesture, SpeechGesture );

bool DSSpeechGesture::Start()
{
	if (! SpeechGesture::Start() )
		return false;

	m_pTTS = SelfInstance::GetInstance()->FindService<TextToSpeech>();
	if ( m_pTTS == NULL )
	{
		Log::Error( "DSSPeechGesture", "TextToSpeech service is missing." );
		return false;
	}

	m_pTTS->GetVoices( DELEGATE( DSSpeechGesture, OnVoices, Voices *, this ) );
	return true;
}

bool DSSpeechGesture::Stop()
{
	PopAllRequests();
	// wait for our active buffer to stop..
	while( m_pBuffer.IsValid() )
	{
		ThreadPool::Instance()->ProcessMainThread();
		boost::this_thread::yield();
	}

	return true;
}

void DSSpeechGesture::InitDS()
{
	if (sm_pDirectSound == NULL)
	{
		DirectSoundCreate8(NULL, &sm_pDirectSound, NULL);
		HRESULT hr = sm_pDirectSound->SetCooperativeLevel(GetDesktopWindow(), DSSCL_PRIORITY);
		if (!SUCCEEDED(hr))
			Log::Error("DSSpeechGesture", "SetCooperativeLevel failed: %S", DXGetErrorString(hr));
	}
}

bool DSSpeechGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
	if ( PushRequest( a_Callback, a_Params ) )
		StartSpeech();

	return true;
}

void DSSpeechGesture::StartSpeech()
{
	if ( m_pVoices != NULL )
	{
		bool bSuccess = false;

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
			pTTS->ToSound( text, DELEGATE( DSSpeechGesture, OnSpeechData, Sound *, this ) );
			bSuccess = true;
		}
		else
			Log::Error( "DSSpeechGesture", "No TextToSpeech service available." );

		if (! bSuccess )
			OnSpeechDone();
	}
}

bool DSSpeechGesture::Abort()
{
	if ( m_pBuffer.IsValid() )
		return false;
	return true;
}

void DSSpeechGesture::OnVoices( Voices * a_pVoices )
{
	m_pVoices = a_pVoices;
	if ( ActiveRequest() != NULL )
		StartSpeech();
}

void DSSpeechGesture::OnSpeechData( Sound * a_pSound )
{
	Request * pReq = ActiveRequest();

	InitDS();

	// make and play a secondary buffer using the provided WAV file..
	if (a_pSound != NULL )
	{
		// Set up WAV format structure. 
		WAVEFORMATEX wfx; 
		memset(&wfx, 0, sizeof(WAVEFORMATEX)); 
		wfx.wFormatTag = WAVE_FORMAT_PCM; 
		wfx.nChannels = a_pSound->GetChannels();
		wfx.nSamplesPerSec = a_pSound->GetRate();
		wfx.wBitsPerSample = a_pSound->GetBits();
		wfx.nBlockAlign = (wfx.wBitsPerSample / 8) * wfx.nChannels;
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; 

		// Set up DSBUFFERDESC structure. 
		DSBUFFERDESC dsbdesc; 
		memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
		dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
		dsbdesc.dwFlags = DSBCAPS_GLOBALFOCUS; 
		dsbdesc.dwBufferBytes = a_pSound->GetWaveData().size(); 
		dsbdesc.lpwfxFormat = &wfx; 

		m_pBuffer.Release();

		// Create buffer. 
		HRESULT hr = sm_pDirectSound->CreateSoundBuffer(&dsbdesc, &m_pBuffer, NULL);
		if (SUCCEEDED(hr)) 
		{
			void * pLock = NULL; DWORD lockBytes = 0;
			hr = m_pBuffer->Lock( 0, a_pSound->GetWaveData().size(), &pLock, &lockBytes, NULL, NULL, 0 );
			if ( SUCCEEDED(hr) )
			{
				memcpy( pLock, a_pSound->GetWaveData().c_str(), a_pSound->GetWaveData().size() );
				m_pBuffer->Unlock( pLock, lockBytes, NULL, 0 );
			}

			// cache the sound buffer for next time..
#if ENABLE_SOUND_CACHE
			m_BufferCache[ pReq->m_Params["text"] ] = m_pBuffer;
#endif

			m_pBuffer->SetCurrentPosition( 0 );
			m_pBuffer->SetVolume( DSBVOLUME_MAX );
			hr = m_pBuffer->Play( 0, 0, 0 );
			if (! SUCCEEDED( hr ) )
				Log::Error( "DSSpeechGesture", "Play failed: %S", DXGetErrorString( hr ) );

			SelfInstance::GetInstance()->GetSensorManager()->PauseSensorType(AudioData::GetStaticRTTI().GetName());
			ThreadPool::Instance()->InvokeOnThread( VOID_DELEGATE( DSSpeechGesture, WaitOnBuffer, this ) );
		}
		else
		{
			Log::Error( "DSSpeechGesture", "Failed to make sound buffer." );
			OnSpeechDone();
		}

		delete a_pSound;
	}
	else
	{
		Log::Error( "DSSpeechGesture", "Failed to load WAV data." );
		OnSpeechDone();
	}
}

void DSSpeechGesture::WaitOnBuffer()
{
	DWORD status = DSBSTATUS_PLAYING; 
	try {
		while( status == DSBSTATUS_PLAYING )
		{
			tthread::this_thread::sleep_for( tthread::chrono::milliseconds( 10 ) );
			m_pBuffer->GetStatus( &status );
		}
	}
	catch(...)
	{
		Log::Error( "DSSpeechGesture", "Exception caught." );
	}

	ThreadPool::Instance()->InvokeOnMain( VOID_DELEGATE( DSSpeechGesture, OnSpeechDone, this ) );
}

void DSSpeechGesture::OnSpeechDone()
{
	SelfInstance::GetInstance()->GetSensorManager()->ResumeSensorType(AudioData::GetStaticRTTI().GetName());

	m_pBuffer.Release();

	// start the next speech if we have any..
	if ( PopRequest() )
		StartSpeech();
}

