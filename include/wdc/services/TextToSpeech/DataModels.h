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

#ifndef WDC_TEXT_TO_SPEECH_MODELS_H
#define WDC_TEXT_TO_SPEECH_MODELS_H

#include "utils/ISerializable.h"

enum AudioFormatType
{
	AF_OGG = 0,
	AF_WAV,                     //Currently used
	AF_FLAC
};

struct Voice : public ISerializable
{
	RTTI_DECL();

	std::string m_Name;
	std::string m_Language;
	std::string m_Gender;
	std::string m_URL;

	virtual void Serialize(Json::Value & json)
	{
		json["name"] = m_Name;
		json["language"] = m_Language;
		json["gender"] = m_Gender;
		json["url"] = m_URL;
	}

	virtual void Deserialize(const Json::Value & json)
	{
		m_Name = json["name"].asString();
		m_Language = json["language"].asString();
		m_Gender = json["gender"].asString();
		m_URL = json["url"].asString();
	}
};

struct Voices : public ISerializable
{
	RTTI_DECL();

	std::vector< Voice > m_Voices;

	virtual void Serialize(Json::Value & json)
	{
		SerializeVector( "voices", m_Voices, json );
	}

	virtual void Deserialize(const Json::Value & json)
	{
		DeserializeVector( "voices", json, m_Voices );
	}
};


#endif // WDC_TEXT_TO_SPEECH_MODELS_H

