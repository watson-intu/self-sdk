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

#ifndef SELF_DATAMODELS_H
#define SELF_DATAMODELS_H

#include "utils/ISerializable.h"

struct WDC_API Documents : public ISerializable
{
    std::string                 m_Id;
    std::string                 m_Title;
    std::string                 m_ContentHTML;
    std::string                 m_Body;
    std::string                 m_FileName;
    std::string                 m_SourceDocId;
    std::string                 m_IndexTimeStamp;
    int                         m_TextSize;
    int                         m_Sequence;
    std::vector<std::string>    m_SearchText;


    virtual void Serialize(Json::Value & json)
    {
        json["id"] = m_Id;
        json["title"] = m_Title;
        json["contentHtml"] = m_ContentHTML;
        json["body"] = m_Body;
        json["fileName"] = m_FileName;
        json["sourceDocId"] = m_SourceDocId;
        json["indexedTimeStamp"] = m_IndexTimeStamp;
        json["sequence"] = m_Sequence;
        json["textSize"] = m_TextSize;
        SerializeVector("searchText", m_SearchText, json);
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("id") )
            m_Id = json["id"].asString();
        if( json.isMember("title") )
            m_Title = json["title"].asString();
        if( json.isMember("contentHtml") )
            m_ContentHTML = json["contentHtml"].asString();
        if( json.isMember("body") )
            m_Body = json["body"].asString();
        if( json.isMember("fileName") )
            m_FileName = json["fileName"].asString();
        if( json.isMember("sourceDocId") )
            m_SourceDocId = json["sourceDocId"].asString();
        if( json.isMember("indexedTimeStamp") )
            m_IndexTimeStamp = json["indexedTimeStamp"].asString();
        if( json.isMember("sequence") )
            m_Sequence = json["sequence"].asInt();
        if( json.isMember("textSize") )
            m_TextSize = json["textSize"].asInt();

        DeserializeVector("searchText", json, m_SearchText);
    }
};

struct WDC_API RetrieveAndRankResponse : public ISerializable
{
    RTTI_DECL();

    Json::Value   m_Response;
    std::vector<Documents>  m_Docs;
    int                     m_NumFound;
    int                     m_Start;

    virtual void Serialize(Json::Value & json)
    {
        json["response"] = m_Response;
        m_Response["numFound"] = m_NumFound;
        m_Response["start"] = m_Start;
        SerializeVector("docs", m_Docs, m_Response);
    }

    virtual void Deserialize(const Json::Value & json)
    {
        m_Response = json["response"];
        if( m_Response.isMember("numFound"))
            m_NumFound = m_Response["numFound"].asInt();

        if( m_Response.isMember("start"))
            m_Start = m_Response["start"].asInt();

        if( m_Response.isMember("docs"))
            DeserializeVector("docs", m_Response, m_Docs);
    }


};

#endif //SELF_DATAMODELS_H
