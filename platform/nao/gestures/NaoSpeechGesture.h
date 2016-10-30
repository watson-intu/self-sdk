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

#ifndef NAO_SPEECH_GESTURE_H
#define NAO_SPEECH_GESTURE_H

#include "gestures/SpeechGesture.h"

#pragma warning(disable:4275)
#include "qi/session.hpp"

//! This gesture wraps the local speech synthesis so the self can speak
class NaoSpeechGesture : public SpeechGesture
{
public:
	RTTI_DECL();

	//! Construction
	NaoSpeechGesture() : m_Language("English")
	{}

	//! ISerializable
	void Serialize(Json::Value & json);
	void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Start();
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

private:
	//! Callbacks
	void SpeechThread( Request * a_pReq );
	void DoSpeechThread( Request * a_pReq );
	void OnSpeechDone( Request * a_pReq );
	bool DoAbort();

	//! Types
	typedef std::map< std::string, std::string >	LanguageMap;

	//! Data
	std::string		m_Language;
	LanguageMap		m_Languages;
};


#endif //SPEECH_GESTURE_H
