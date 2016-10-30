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

#ifndef NAO_SOUND_GESTURE_H
#define NAO_SOUND_GESTURE_H

#include "gestures/SoundGesture.h"
#include "utils/IWebClient.h"

//! This gesture wraps a sound effect to play back
class NaoSoundGesture : public SoundGesture
{
public:
	RTTI_DECL();

	NaoSoundGesture() : m_Player( NULL ), m_bPlaying( false ), m_bStreaming( false )
	{}

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

private:
	//! Data
	FILE *			m_Player;
	volatile bool	m_bPlaying;
	volatile bool	m_bStreaming;
	GestureDelegate m_Callback;

	void Play();
	void PlayThread();
	void PlayDone();

	void OnStreamState( IWebClient * );
	void OnStreamData( IWebClient::RequestData * );
};


#endif //SELF_SOUND_GESTURE_H
