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

#ifndef WDC_SPEECH_TO_TEXT_MODELS_H
#define WDC_SPEECH_TO_TEXT_MODELS_H

#include "utils/ISerializable.h"

struct SpeechModel : public ISerializable
{
	RTTI_DECL();

	std::string m_Name;
	int			m_Rate;
	std::string m_Language;
	std::string m_Description;
	std::string m_URL;

	virtual void Serialize(Json::Value & json)
	{
		json["name"] = m_Name;
		json["rate"] = m_Rate;
		json["language"] = m_Language;
		json["description"] = m_Description;
		json["url"] = m_URL;
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_Name = json["name"].asString();
		m_Rate = json["rate"].asInt();
		m_Language = json["language"].asString();
		m_Description = json["description"].asString();
		m_URL = json["url"].asString();
	}
};

struct SpeechModels : ISerializable
{
	RTTI_DECL();

	std::vector<SpeechModel>	m_Models;

	virtual void Serialize(Json::Value & json)
	{
		SerializeVector( "models", m_Models, json );
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		DeserializeVector( "models", json, m_Models );
	}
};

struct WordConfidence : public ISerializable
{
	RTTI_DECL();

	std::string m_Word;
	double		m_Confidence;

	virtual void Serialize(Json::Value & json)
	{
		json[0] = m_Word;
		json[1] = m_Confidence;
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_Word = json[0].asString();
		m_Confidence = json[1].asDouble();
	}
};

struct TimeStamp : public ISerializable
{
	RTTI_DECL();

	std::string m_Word;
	double		m_Start;
	double		m_End;

	virtual void Serialize(Json::Value & json)
	{
		json[0] = m_Word;
		json[1] = m_Start;
		json[2] = m_End;
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_Word = json[0].asString();
		m_Start = json[1].asDouble();
		m_End = json[2].asDouble();
	}
};

struct SpeechAlt : public ISerializable
{
	RTTI_DECL();

	std::string m_Transcript;
	double		m_Confidence;
	std::vector<TimeStamp> 
				m_Timestamps;
	std::vector<WordConfidence> 
				m_WordConfidence;

	virtual void Serialize(Json::Value & json)
	{
		json["transcript"] = m_Transcript;
		json["confidence"] = m_Confidence;
		
		SerializeVector( "timestamps", m_Timestamps, json );
		SerializeVector( "word_confidence", m_WordConfidence, json );
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_Transcript = json["transcript"].asString();
		m_Confidence = json["confidence"].asDouble();

		DeserializeVector( "timestamps", json, m_Timestamps );
		DeserializeVector( "word_confidence", json, m_WordConfidence );
	}
};

struct SpeechResult : public ISerializable
{
	RTTI_DECL();

	bool	m_Final;
	std::vector<SpeechAlt> 
			m_Alternatives;

	virtual void Serialize(Json::Value & json)
	{
		json["final"] = m_Final;
		SerializeVector( "alternatives", m_Alternatives, json );
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_Final = json["final"].asBool();
		DeserializeVector( "alternatives", json, m_Alternatives );
	}

};

struct RecognizeResults : public ISerializable
{
	RTTI_DECL();

	int m_ResultIndex;
	std::vector<SpeechResult> m_Results;
	std::string m_Language;

	RecognizeResults() : m_ResultIndex( 0 )
	{}

	RecognizeResults( const std::vector<SpeechResult> & results) : m_ResultIndex( 0 )
	{
		m_Results = results;
	}

	bool HasResult() const
	{
		return m_Results.size() > 0 && m_Results[0].m_Alternatives.size() > 0;
	}
	bool HasFinalResult() const
	{
		return HasResult() && m_Results[0].m_Final;
	}
	double GetConfidence() const
	{
		return HasResult() ? m_Results[0].m_Alternatives[0].m_Confidence : -1.0f;
	}

	void SetLanguage(std::string & a_Language)
	{
		m_Language = a_Language;
	}

	std::string & GetLanguage() 
	{
		return m_Language;
	}

	virtual void Serialize(Json::Value & json)
	{
		json["result_index"] = m_ResultIndex;
		SerializeVector( "results", m_Results, json );
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_ResultIndex = json["result_index"].asInt();
		DeserializeVector( "results", json, m_Results );
	}
};

struct SpeechAudioData : public ISerializable
{
	std::string			m_PCM;
	int					m_Rate;
	int					m_Channels;
	int					m_Bits;
	float				m_Level;

	SpeechAudioData() : m_Rate( 0 ), m_Channels( 0 ), m_Bits( 0 ), m_Level( 0.0f )
	{}

	virtual void Serialize(Json::Value & json)
	{
		json["m_PCM"] = m_PCM;
		json["m_Rate"] = m_Rate;
		json["m_Channels"] = m_Channels;
		json["m_Bits"] = m_Bits;
		json["m_Level"] = m_Level;
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_PCM = json["m_PCM"].asString();
		m_Rate = json["m_Rate"].asInt();
		m_Channels = json["m_Channels"].asInt();
		m_Bits = json["m_Bits"].asInt();
		m_Level = json["m_Level"].asFloat();
	}

};

#endif // STT_MODELS_H

