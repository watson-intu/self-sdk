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

#include "DSMicrophone.h"
#include "utils/ThreadPool.h"
#include "utils/WatsonException.h"
#include "sensors/AudioData.h"
#include "utils/QIRef.h"

#include <mmsystem.h>
#include <dsound.h>

#pragma comment(lib, "dsound" )
#pragma comment(lib, "dxguid" )

REG_OVERRIDE_SERIALIZABLE( Microphone, DSMicrophone );
REG_SERIALIZABLE( DSMicrophone );
RTTI_IMPL(DSMicrophone, Microphone);

DSMicrophone::DSMicrophone() : m_CaptureAudio( false ), m_Paused( 0 ), m_Error( false ), m_CaptureStopped( true )
{
	for(int i=0;i<sizeof(m_CaptureEvents)/sizeof(m_CaptureEvents[0]);++i)
		m_CaptureEvents[i] = CreateEvent( NULL, FALSE, FALSE, NULL );
}

DSMicrophone::~DSMicrophone()
{
	// stop our streaming thread..
	m_CaptureAudio = false;
	while(! m_CaptureStopped )
		tthread::this_thread::yield();

}

bool DSMicrophone::OnStart()
{
	if (! m_CaptureAudio )
	{
		m_CaptureAudio = true;
		m_CaptureStopped = false;

		ThreadPool::Instance()->InvokeOnThread<void *>( DELEGATE( DSMicrophone, CaptureAudio, void *, this), 0 );
		return true;
	}
	return false;
}

bool DSMicrophone::OnStop()
{
	m_CaptureAudio = false;
	while(! m_CaptureStopped )
		tthread::this_thread::yield();
	return true;
}

void DSMicrophone::OnPause()
{
	m_Paused++;
}

void DSMicrophone::OnResume()
{
	m_Paused--;
}

void DSMicrophone::CaptureAudio( void * )
{
	QIRef<LPDIRECTSOUNDCAPTURE>			pDSC;					// DirectSound Capture device
	QIRef<LPDIRECTSOUNDCAPTUREBUFFER>	pDSCB;					// capture buffer
	QIRef<LPDIRECTSOUNDNOTIFY>			pDSSN;					// capture notify object
							
	if (DirectSoundCaptureCreate(NULL, &pDSC, NULL) != DS_OK)
	{
		Log::Error("DSMicrophone", "Could not detect microphone on start up, closing stream");
		m_CaptureStopped = true;
		return;
	}

	WAVEFORMATEX wfx;
	memset(&wfx,0,sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = m_RecordingChannels;
	wfx.nSamplesPerSec = m_RecordingHZ;
	wfx.wBitsPerSample = m_RecordingBits;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	DSCBUFFERDESC dscbDesc;
	memset(&dscbDesc,0,sizeof(DSCBUFFERDESC));
	dscbDesc.dwSize = sizeof(DSCBUFFERDESC);
	dscbDesc.dwFlags = 0;
	dscbDesc.dwBufferBytes = wfx.nAvgBytesPerSec / 4;
	dscbDesc.dwReserved = 0;
	dscbDesc.lpwfxFormat = &wfx;

	if ( pDSC->CreateCaptureBuffer( &dscbDesc, &pDSCB, NULL ) != DS_OK )
		throw WatsonException( "DSMicrophone::CaptureAudio(): CreateCaptureBuffer failed." );

	// lock the buffer and clear the contents
	void * pLock = NULL;
	DWORD lockSize = 0;
	if ( pDSCB->Lock( 0, dscbDesc.dwBufferBytes, &pLock, &lockSize, NULL, NULL, 0 ) != DS_OK )
		throw WatsonException( "DSMicrophone::CaptureAudio(): Lock failed." );

	memset( pLock, 0, lockSize );
	pDSCB->Unlock( pLock, lockSize, 0, 0 );

	DSBPOSITIONNOTIFY dscbpn[ 2 ];
	dscbpn[ 0 ].dwOffset = 0;
	dscbpn[ 0 ].hEventNotify = m_CaptureEvents[ 0 ];
	dscbpn[ 1 ].dwOffset = dscbDesc.dwBufferBytes / 2;
	dscbpn[ 1 ].hEventNotify = m_CaptureEvents[ 1 ];

	pDSCB->QueryInterface( IID_IDirectSoundNotify, (void **)&pDSSN );
	if ( pDSSN->SetNotificationPositions( 2, dscbpn ) != DS_OK )
		throw WatsonException( "DSMicrophone::CaptureAudio(): SetNotificationPositions failed." );

	// start the capture
	if ( pDSCB->Start( DSCBSTART_LOOPING ) != DS_OK )
		throw WatsonException( "DSMicrophone::CaptureAudio(): Start() failed." ); 

	int blockSize = (int)dscbDesc.dwBufferBytes / 2;
	while( m_CaptureAudio )
	{
		int eventIndex = WaitForMultipleObjects( sizeof(m_CaptureEvents)/sizeof(m_CaptureEvents[0]), m_CaptureEvents, false, INFINITE ) - WAIT_OBJECT_0;

		if (m_Paused <= 0 )
		{
			void *	locked = NULL;
			DWORD	lockedSize = 0;

			if ( eventIndex == 0 )
			{
				// lock the last half of the capture buffer
				if ( pDSCB->Lock( blockSize, blockSize, &locked, &lockedSize, NULL, NULL, 0 ) != DS_OK )
					throw WatsonException( "DSMicrophone::CaptureAudio(): Lock failed." );
			}
			else
			{
				// lock the first half of the capture buffer
				if ( pDSCB->Lock( 0, blockSize, &locked, &lockedSize, NULL, NULL, 0 ) != DS_OK )
					throw WatsonException( "DSMicrophone::CaptureAudio(): Lock failed." );
			}

			// send the capture buffer to the AudioListener
			ThreadPool::Instance()->InvokeOnMain( DELEGATE(DSMicrophone, SendAudio, AudioData *, this),
				new AudioData( std::string( (const char *)locked, lockedSize), m_RecordingHZ, m_RecordingChannels, m_RecordingBits ) );

			// unlock the capture buffer
			pDSCB->Unlock( locked, lockedSize, NULL, 0 );
		}
	}

	m_CaptureStopped = true;
}

void DSMicrophone::SendAudio( AudioData * a_pData )
{
	// now send the data to all subscribers for this microphone..
	SendData(a_pData);
}

