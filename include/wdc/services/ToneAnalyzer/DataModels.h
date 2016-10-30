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

#ifndef WDC_TONE_DATAMODELS_H
#define WDC_TONE_DATAMODELS_H

#include "utils/ISerializable.h"

struct WDC_API Tone : public ISerializable
{
    double m_Score;
    std::string m_ToneId;
    std::string m_ToneName;

    virtual void Serialize(Json::Value & json)
    {
        json["score"] = m_Score;
        json["tone_id"] = m_ToneId;
        json["tone_name"] = m_ToneName;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("score") )
            m_Score = json["score"].asDouble();
        if( json.isMember("tone_id") )
            m_ToneId = json["tone_id"].asString();
        if( json.isMember("tone_name") )
            m_ToneName = json["tone_name"].asString();
    }
};

struct WDC_API ToneCategory : public ISerializable
{
    std::string m_CategoryId;
    std::string m_CategoryName;
    std::vector<Tone> m_Tones;

    virtual void Serialize(Json::Value & json)
    {
        json["category_id"] = m_CategoryId;
        json["category_name"] = m_CategoryName;
        SerializeVector("tones", m_Tones, json);
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("category_id") )
            m_CategoryId = json["category_id"].asString();
        if( json.isMember("category_name") )
            m_CategoryName = json["category_name"].asString();

        DeserializeVector("tones", json, m_Tones);

    }
};

struct WDC_API DocumentTones : public ISerializable
{
    RTTI_DECL();

    Json::Value m_DocumentTone;
    std::vector<ToneCategory> m_ToneCategories;

    virtual void Serialize(Json::Value & json)
    {
        json["document_tone"] = m_DocumentTone;
        SerializeVector("tone_categories", m_ToneCategories, m_DocumentTone);
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("document_tone"))
            m_DocumentTone = json["document_tone"];

        DeserializeVector("tone_categories",m_DocumentTone, m_ToneCategories);

    }
};

#endif //WDC_TONE_DATAMODELS_H
