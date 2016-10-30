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


#include "DSSoundGesture.h"
#include "sensors/AudioData.h"
#include "utils/ThreadPool.h"
#include "utils/Sound.h"
#include "utils/IWebClient.h"

#include "SelfInstance.h"

#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#include <dxerr.h>

#include <boost/thread.hpp>

REG_OVERRIDE_SERIALIZABLE(SoundGesture, DSSoundGesture);
REG_SERIALIZABLE(DSSoundGesture);
RTTI_IMPL(DSSoundGesture, SoundGesture);

bool DSSoundGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
	if (PushRequest(a_Callback, a_Params))
		Play();

	return true;
}

bool DSSoundGesture::Abort()
{
	return false;
}

void DSSoundGesture::Play()
{
	Request * pReq = ActiveRequest();
	if (pReq != NULL)
	{
		if (pReq->m_Params.GetData().isMember("sound"))
			m_Sound = pReq->m_Params["sound"].asString();
		if (pReq->m_Params.GetData().isMember("volume"))
			m_fVolume = pReq->m_Params["volume"].asFloat();
		if (pReq->m_Params.GetData().isMember("pan"))
			m_fPan = pReq->m_Params["pan"].asFloat();

		ThreadPool::Instance()->InvokeOnThread(VOID_DELEGATE(DSSoundGesture, PlayThread, this));
	}
}

void DSSoundGesture::PlayThread()
{
	Request * pReq = ActiveRequest();

	DSSpeechGesture::InitDS();

	bool bWebRequest = StringUtil::StartsWith(m_Sound, "http://", true) ||
		StringUtil::StartsWith(m_Sound, "https://", true);

	if (bWebRequest)
	{
		m_bStreaming = true;

		IWebClient * pClient = IWebClient::Create();
		pClient->SetURL(m_Sound);
		pClient->SetStateReceiver(DELEGATE(DSSoundGesture, OnStreamState, IWebClient *, this));
		pClient->SetDataReceiver(DELEGATE(DSSoundGesture, OnStreamData, IWebClient::RequestData *, this));

		if (pClient->Send())
		{
			while (m_bStreaming)
				boost::this_thread::sleep(boost::posix_time::milliseconds(50));
		}
		else
		{
			Log::Error("DSSoundGesture", "Unable to connect to url: %s", m_Sound.c_str());
			m_bStreaming = false;
		}
	}
	else
	{
		// make and play a secondary buffer using the provided WAV file..
		Sound sound;
		if (sound.LoadFromFile(m_Sound))
		{
			// Set up WAV format structure. 
			WAVEFORMATEX wfx;
			memset(&wfx, 0, sizeof(WAVEFORMATEX));
			wfx.wFormatTag = WAVE_FORMAT_PCM;
			wfx.nChannels = sound.GetChannels();
			wfx.nSamplesPerSec = sound.GetRate();
			wfx.wBitsPerSample = sound.GetBits();
			wfx.nBlockAlign = (wfx.wBitsPerSample / 8) * wfx.nChannels;
			wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

			// Set up DSBUFFERDESC structure. 
			DSBUFFERDESC dsbdesc;
			memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
			dsbdesc.dwSize = sizeof(DSBUFFERDESC);
			dsbdesc.dwFlags = DSBCAPS_GLOBALFOCUS;
			dsbdesc.dwBufferBytes = sound.GetWaveData().size();
			dsbdesc.lpwfxFormat = &wfx;

			// Create buffer. 
			HRESULT hr = DSSpeechGesture::sm_pDirectSound->CreateSoundBuffer(&dsbdesc, &m_pBuffer, NULL);
			if (SUCCEEDED(hr))
			{
				void * pLock = NULL; DWORD lockBytes = 0;
				hr = m_pBuffer->Lock(0, sound.GetWaveData().size(), &pLock, &lockBytes, NULL, NULL, 0);
				if (SUCCEEDED(hr))
				{
					memcpy(pLock, sound.GetWaveData().c_str(), sound.GetWaveData().size());
					m_pBuffer->Unlock(pLock, lockBytes, NULL, 0);
				}

				// cache the sound buffer for next time..
#if ENABLE_SOUND_CACHE
				m_BufferCache[pReq->m_Params["text"]] = pDSB;
#endif

				m_pBuffer->SetCurrentPosition(0);
				// TODO: apply m_fVolume & m_fPan
				m_pBuffer->SetVolume(DSBVOLUME_MAX);
				hr = m_pBuffer->Play(0, 0, 0);
				if (!SUCCEEDED(hr))
					Log::Error("DSSoundGesture", "Play failed: %S", DXGetErrorString(hr));

				DWORD status = DSBSTATUS_PLAYING;
				try {
					while (status == DSBSTATUS_PLAYING)
					{
						boost::this_thread::sleep(boost::posix_time::milliseconds(10));
						m_pBuffer->GetStatus(&status);
					}
				}
				catch (...)
				{
					Log::Error("DSSoundGesture", "Exception caught.");
				}

			}
			else
			{
				Log::Error("DSSoundGesture", "Failed to make sound buffer.");
			}
		}
		else
		{
			Log::Error("DSSoundGesture", "Failed to load WAV data.");
		}
	}

	ThreadPool::Instance()->InvokeOnMain(VOID_DELEGATE(DSSoundGesture, OnPlayDone, this));
}

void DSSoundGesture::OnPlayDone()
{
	m_pBuffer.Release();

	// start the next speech if we have any..
	if (PopRequest())
		Play();
}

void DSSoundGesture::OnStreamState( IWebClient * a_pClient )
{
	if ( a_pClient->GetState() == IWebClient::DISCONNECTED 
		|| a_pClient->GetState() == IWebClient::CLOSED )
	{
		Log::Debug( "DSSOundGesture", "Sound stream closed." );
		m_bStreaming = false;
		delete a_pClient;
	}
}

static bool ParseContentType( const std::string & a_ContentType, WAVEFORMATEX & wfx )
{
	// Set up WAV format structure. 
	memset(&wfx, 0, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	wfx.nChannels = 1;
	wfx.nSamplesPerSec = 16000;
	wfx.wBitsPerSample = 16;

	// parse the content type, excpecting something like audio/L16;rate=16000
	std::vector<std::string> parts;
	StringUtil::Split( a_ContentType, ";", parts );
	if ( parts.size() == 0 )
		return false;
	if ( StringUtil::Compare( parts[0], "audio/L8", true ) == 0 )
		wfx.wBitsPerSample = 8;
	else if ( StringUtil::Compare( parts[0], "audio/L16", true ) != 0 )
		return false;

	for(size_t i=1;i<parts.size();++i)
	{
		std::vector<std::string> kv;
		StringUtil::Split( parts[i], "=", kv );
		if ( kv.size() != 2 )
			continue;

		if ( StringUtil::Compare( kv[0], "rate", true ) == 0 )
			wfx.nSamplesPerSec = atoi( kv[1].c_str() );
		else if ( StringUtil::Compare( kv[0], "channels", true) == 0 )
			wfx.nChannels = atoi( kv[1].c_str() );
	}

	wfx.nBlockAlign = (wfx.wBitsPerSample / 8) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	return true;
}

void DSSoundGesture::OnStreamData( IWebClient::RequestData * a_pData )
{
	if (! m_pBuffer.IsValid() )
	{
		const std::string & contentType = a_pData->m_Headers["Content-Type"];

		// Set up WAV format structure. 
		WAVEFORMATEX wfx;
		if ( ParseContentType( contentType, wfx ) )
		{
			// Set up DSBUFFERDESC structure. 
			DSBUFFERDESC dsbdesc;
			memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
			dsbdesc.dwSize = sizeof(DSBUFFERDESC);
			dsbdesc.dwFlags = DSBCAPS_GLOBALFOCUS;
			dsbdesc.dwBufferBytes = wfx.nAvgBytesPerSec * 1;		
			dsbdesc.lpwfxFormat = &wfx;

			m_nWriteEnd = dsbdesc.dwBufferBytes;
			m_nWritePos = 0;

			// Create buffer. 
			HRESULT hr = DSSpeechGesture::sm_pDirectSound->CreateSoundBuffer(&dsbdesc, &m_pBuffer, NULL);
			if (SUCCEEDED(hr))
			{
				void * pBuffer; DWORD bytes;
				hr = m_pBuffer->Lock( 0, m_nWriteEnd, &pBuffer, &bytes, NULL, NULL, 0 );
				if ( SUCCEEDED(hr) )
				{
					memset( pBuffer, 0, bytes );
					m_pBuffer->Unlock( pBuffer, bytes, 0, 0 );
				}
				else
					m_pBuffer.Release();
			}
			else
			{
				Log::Error( "DSSooundGesture", "Failed to create DS buffer." );
			}
		}
		else
		{
			Log::Error( "DSSoundGesture", "Unsupported content type: %s", contentType.c_str() );
		}
	}

	if ( m_pBuffer.IsValid() && a_pData->m_Content.size() > 0 )
	{
		void * pLock[2];
		DWORD bytes[2];

		const void * pAudio = a_pData->m_Content.c_str();
		DWORD audio_bytes = a_pData->m_Content.size();

		//Log::DebugLow( "DSSpeechGesture", "Lock(), m_nWritePos = %u, Content Size = %u", m_nWritePos, a_pData->m_Content.size() );
		HRESULT hr = m_pBuffer->Lock( m_nWritePos, audio_bytes, &pLock[0], &bytes[0], &pLock[1], &bytes[1], 0 );
		if ( hr == DSERR_BUFFERLOST ) 
		{
			Log::Status( "DSSoundGesture", "Restoring lost buffer." );
			m_pBuffer->Restore();
			hr = m_pBuffer->Lock( m_nWritePos, audio_bytes, &pLock[0], &bytes[0], &pLock[1], &bytes[1], 0 );
		}
		if ( SUCCEEDED(hr) )
		{
			assert( (bytes[0] + bytes[1]) == audio_bytes );
			memcpy( pLock[0], &a_pData->m_Content[0], bytes[0] );
			if ( bytes[1] > 0 )
				memcpy( pLock[1], &a_pData->m_Content[0] + bytes[0], bytes[1] );

			m_pBuffer->Unlock( pLock[0], bytes[0], pLock[1], bytes[1] );
			m_nWritePos += audio_bytes;

			while ( m_nWritePos >= m_nWriteEnd )
				m_nWritePos -= m_nWriteEnd;

			// don't start playing until we have filled half of the buffer..
			if ( m_nWritePos > (m_nWriteEnd / 2) )
			{
				// start playing the buffer if not playing..
				DWORD status = 0;
				m_pBuffer->GetStatus( &status );
				if ( (status & DSBSTATUS_PLAYING) == 0 )
				{
					m_pBuffer->SetCurrentPosition( 0 );
					m_pBuffer->SetVolume(DSBVOLUME_MAX);
					m_pBuffer->Play( 0, 0, DSBPLAY_LOOPING );
				}
			}
		}
		else 
		{
			Log::Error( "DSSoundGesture", "Failed to lock buffer." );
		}
	}
}

