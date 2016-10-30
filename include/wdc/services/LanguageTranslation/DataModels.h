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

#ifndef WDC_LT_MODELS_H
#define WDC_LT_MODELS_H

#include "utils/ISerializable.h"
#include "WDCLib.h"

struct WDC_API Translation : public ISerializable
{
    RTTI_DECL();

    std::string m_Translation;

    virtual void Serialize(Json::Value & json)
    {
        json["translation"] = m_Translation;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if(json.isMember("translation"))
            m_Translation = json["translation"].asString();
    }
};

struct WDC_API Translations : public ISerializable
{
    RTTI_DECL();

    int m_WordCount;
    int m_CharacterCount;
    std::vector<Translation> m_Translations;

    virtual void Serialize(Json::Value & json)
    {
        json["word_count"] = m_WordCount;
        json["character_count"] = m_CharacterCount;
        SerializeVector( "translations", m_Translations, json );
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if(json.isMember("word_count"))
            m_WordCount = json["word_count"].asInt();
        if(json.isMember("character_count"))
            m_CharacterCount = json["character_count"].asInt();

        DeserializeVector( "translations", json, m_Translations );
    }
};

struct WDC_API Language : public ISerializable
{
    RTTI_DECL();

    std::string m_Language;
    std::string m_Name;

    virtual void Serialize(Json::Value & json)
    {
        json["language"] = m_Language;
        json["name"] = m_Name;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if(json.isMember("language"))
            m_Language = json["language"].asString();
        if(json.isMember("name"))
            m_Name = json["name"].asString();
    }
};

struct WDC_API Languages : public ISerializable
{
    RTTI_DECL();

    std::vector<Language> m_Languages;

    virtual void Serialize(Json::Value & json)
    {
        SerializeVector( "languages", m_Languages, json );
    }

    virtual void Deserialize(const Json::Value & json)
    {
        DeserializeVector( "languages", json, m_Languages);
    }
};

struct WDC_API IdentifiedLanguage : public ISerializable
{
    RTTI_DECL();

    double m_Confidence;
    std::string m_Language;

    virtual void Serialize(Json::Value & json)
    {
        json["confidence"] = m_Confidence;
        json["language"] = m_Language;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if(json.isMember("confidence"))
            m_Confidence = json["confidence"].asDouble();
        if(json.isMember("language"))
            m_Language = json["language"].asString();
    }
};

struct WDC_API IdentifiedLanguages : public ISerializable
{
    RTTI_DECL();

    std::vector<IdentifiedLanguage> m_Languages;

    virtual void Serialize(Json::Value & json)
    {
        SerializeVector( "languages", m_Languages, json);
    }

    virtual void Deserialize(const Json::Value & json)
    {
        DeserializeVector( "languages", json, m_Languages );
    }
};


#endif //WDC_LT_MODELS_H
