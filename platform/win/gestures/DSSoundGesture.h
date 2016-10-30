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

#ifndef DS_SOUND_GESTURE_H
#define DS_SOUND_GESTURE_H

#include "utils/IWebClient.h"
#include "gestures/SoundGesture.h"
#include "DSSpeechGesture.h"

class Sound;

//! This gesture wraps DirectSound so the local windows box can speak.
class DSSoundGesture : public SoundGesture
{
public:
	RTTI_DECL();

	//! Construction
	DSSoundGesture() : m_bStreaming( false ), m_nWritePos( 0 ), m_nWriteEnd( 0 )
	{}

	//! IGesture interface
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap & a_Params);
	virtual bool Abort();

private:
	//! Data
	volatile bool				m_bStreaming;
	QIRef<LPDIRECTSOUNDBUFFER>	m_pBuffer;
	unsigned int				m_nWritePos;
	unsigned int				m_nWriteEnd;

	void Play();
	void PlayThread();
	void OnPlayDone();

	void OnStreamState( IWebClient * );
	void OnStreamData( IWebClient::RequestData * );
};


#endif //DS_SOUND_GESTURE_H
