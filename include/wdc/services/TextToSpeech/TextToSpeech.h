/**
* Copyright 2015 IBM Corp. All Rights Reserved.
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

#ifndef WDC_TEXT_TO_SPEECH_H
#define WDC_TEXT_TO_SPEECH_H

#include "services/IService.h"
#include "utils/Sound.h"
#include "utils/DataCache.h"
#include "DataModels.h"
#include "WDCLib.h"

class WDC_API TextToSpeech : public IService
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<Sound *>				ToSoundCallback;
	typedef Delegate<Voices *>				GetVoicesCallback;

	//! Construction
	TextToSpeech();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
	virtual void GetServiceStatus( IService::ServiceStatusCallback a_Callback );

	//! Accessors
	bool IsCacheEnabled() const 
	{
		return m_bCacheEnabled;
	}
	const std::string & GetVoice() const
	{
		return m_Voice;
	}

	//! Mutators
	void SetCacheEnabled( bool a_bEnabled )
	{
		m_bCacheEnabled = a_bEnabled;
	}
	void SetVoice( const std::string & a_Voice )
	{
		m_Voice = a_Voice;
	}

	//! Request all the available voices to use with TextToSpeech, the provided callback
	//! will be invoked with the results.
	void GetVoices( GetVoicesCallback a_Callback );
	//! Request the audio data in the specified format for the provided text.
	void Synthesis( const std::string & a_Text, AudioFormatType a_eFormat, 
		Delegate<const std::string &> a_Callback );
	//! Request a conversion of text to speech, note if the speech is in the local cache
	//! then the callback will be invoked and NULL will be returned.
	void ToSound( const std::string & a_Text, ToSoundCallback a_Callback );

	//! Static
	static std::string & GetFormatName( AudioFormatType a_eFormat );
	static std::string & GetFormatId( AudioFormatType a_eFormat );

private:
	//! This class is responsible for checking whether the service is available or not
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(TextToSpeech* a_pTtsService, ServiceStatusCallback a_Callback);

	private:
		TextToSpeech* m_pTtsService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(Voices* a_pVoices);
	};

	//! Data
	std::string		m_Voice;
};

#endif
