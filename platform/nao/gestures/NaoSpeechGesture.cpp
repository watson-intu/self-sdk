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

#define ENABLE_ANIMATED_SPEECH		1			// define to non-zero to use animated speech
#define ENABLE_AUDIO_PAUSE			1			// define to non-zero to pause audio input while talking

#include "NaoPlatform.h"
#include "NaoSpeechGesture.h"
#include "SelfInstance.h"

#include "utils/ThreadPool.h"
#include "sensors/SensorManager.h"
#include "sensors/AudioData.h"
#include "skills/SkillManager.h"

// override the serialization of SpeechGesture
REG_OVERRIDE_SERIALIZABLE(SpeechGesture, NaoSpeechGesture);
REG_SERIALIZABLE(NaoSpeechGesture);
RTTI_IMPL( NaoSpeechGesture, SpeechGesture );


void NaoSpeechGesture::Serialize(Json::Value & json)
{
	SpeechGesture::Serialize(json);
	json["m_Language"] = m_Language;
}

void NaoSpeechGesture::Deserialize(const Json::Value & json)
{
	SpeechGesture::Deserialize(json);
	if ( json.isMember("m_Language") )
		m_Language = json["m_Language"].asString();
}

bool NaoSpeechGesture::Start()
{
	if (! SpeechGesture::Start() )
		return false;

	qi::AnyObject tts = NaoPlatform::Instance()->GetSession()->service("ALTextToSpeech");
	m_Language = tts.call<std::string>( "getLanguage" );

	// TODO: Get the supported voices and remove those not supported.
	m_Languages["en-US"] = "English";
	m_Languages["es-ES"] = "Spanish";
	m_Languages["ja-JP"] = "Japanese";
	m_Languages["it-IT"] = "Italian";
	m_Languages["de-DE"] = "German";
	m_Languages["fr-FR"] = "French";
	m_Languages["pt-BR"] = "Portuguese";
	m_Languages["ko-KR"] = "Korean";

	return true;
}

bool NaoSpeechGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
	if ( PushRequest( a_Callback, a_Params ) )
	{
#if ENABLE_AUDIO_PAUSE
		SelfInstance::GetInstance()->GetSensorManager()->PauseSensorType(AudioData::GetStaticRTTI().GetName());
#endif
		ThreadPool::Instance()->InvokeOnThread(DELEGATE(NaoSpeechGesture, SpeechThread, Request *, this), ActiveRequest() );
	}

	return true;
}

bool NaoSpeechGesture::Abort()
{
	try
	{
		return DoAbort();
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoSpeechGesture", "Caught Exception: %s", ex.what() );
	}

	return false;
}

bool NaoSpeechGesture::DoAbort()
{
	if (HaveRequests())
	{
		Log::Debug("TextToSpeechSkill", "Abort() invoked.");

#if ENABLE_ANIMATED_SPEECH
		qi::AnyObject tts = NaoPlatform::Instance()->GetSession()->service("ALAnimatedSpeech");
		tts.call<void>("_stopAll", false );

		//TODO: Is this necessary anymore?
		qi::AnyObject motion = NaoPlatform::Instance()->GetSession()->service("ALMotion");
		motion.call<void>( "setBreathEnabled", "Arms", true );
		motion.call<void>( "setBreathEnabled", "Body", true );
#else
		qi::AnyObject tts = NaoPlatform::Instance()->GetSession()->service("ALTextToSpeech");
		tts.call<void>("stopAll");
#endif
		return true;
	}

	return false;
}

void NaoSpeechGesture::SpeechThread( Request * a_pReq )
{
	try {
		DoSpeechThread( a_pReq );
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoSpeechGesture", "Caught Exception: %s", ex.what() );
		a_pReq->m_bError = true;
	}

	ThreadPool::Instance()->InvokeOnMain(DELEGATE(NaoSpeechGesture, OnSpeechDone, Request *, this), a_pReq );
}

void NaoSpeechGesture::DoSpeechThread( Request * a_pReq )
{
	const std::string & text = a_pReq->m_Params["text"].asString();
	const std::string & gender = a_pReq->m_Params["gender"].asString();
	const std::string & language = a_pReq->m_Params["language"].asString();

	qi::AnyObject tts = NaoPlatform::Instance()->GetSession()->service("ALTextToSpeech");

	LanguageMap::iterator iLang = m_Languages.find( language );
	if ( iLang != m_Languages.end() && iLang->second != m_Language )
	{
		tts.call<void>( "setLanguage", iLang->second );
		m_Language = iLang->second;
	}

#if ENABLE_ANIMATED_SPEECH
	if( !NaoPlatform::Instance()->IsExtraMovementDisabled()
		&& !NaoPlatform::Instance()->IsRestPosture() )
	{
		tts = NaoPlatform::Instance()->GetSession()->service("ALAnimatedSpeech");
	}
#endif
	tts.call<void>("say", text);
}

void NaoSpeechGesture::OnSpeechDone( Request * a_pReq )
{
	if (! PopRequest() )
	{
#if ENABLE_AUDIO_PAUSE
		SelfInstance::GetInstance()->GetSensorManager()->ResumeSensorType(AudioData::GetStaticRTTI().GetName());
#endif
	}
	else
		ThreadPool::Instance()->InvokeOnThread(DELEGATE(NaoSpeechGesture, SpeechThread, Request *, this), ActiveRequest() );
}

