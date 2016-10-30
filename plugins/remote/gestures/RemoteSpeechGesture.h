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

#ifndef REMOTE_SPEECH_GESTURE_H
#define REMOTE_SPEECH_GESTURE_H

#include "gestures/SpeechGesture.h"

class Sound;
class TextToSpeech;
struct Voices;

//! This gesture wraps DirectSound so the local windows box can speak.
class RemoteSpeechGesture : public SpeechGesture
{
public:
	RTTI_DECL();

	//! Construction
	RemoteSpeechGesture() : m_pTTS( NULL ), m_pVoices( NULL )
	{}

	//! IGesture interface
	virtual bool Start();
	virtual bool Stop();
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

private:
	void StartSpeech();
	void OnVoices( Voices * a_pVoices );
	void OnSpeechData( Sound * );
	void OnSpeechDone();


	//! Data
	TextToSpeech *				m_pTTS;
	Voices *					m_pVoices;			
};


#endif //SPEECH_GESTURE_H
