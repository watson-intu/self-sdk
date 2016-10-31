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

#ifndef WDC_CONVERSATION_MODELS_H
#define WDC_CONVERSATION_MODELS_H

#include "utils/ISerializable.h"


struct WDC_API ConversationIntent : public ISerializable
{
    std::string m_Intent;
    float m_fConfidence;

    virtual void Serialize(Json::Value & json)
    {
        json["confidence"] = m_fConfidence;
        json["intent"] = m_Intent;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if ( json.isMember("confidence") )
            m_fConfidence = json["confidence"].asFloat();
        if ( json.isMember("intent") )
            m_Intent = json["intent"].asString();
    }
};

struct WDC_API ConversationEntities : public ISerializable
{
    std::string m_Entity; // Example: "joint" or "direction"
    std::string m_Value; // Example: "arm" or "left"

    virtual void Serialize(Json::Value & json)
    {
        json["entity"] = m_Entity;
        json["value"] = m_Value;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if ( json.isMember("entity") )
            m_Entity = json["entity"].asString();
        if ( json.isMember("value") )
            m_Value = json["value"].asString();
    }
};


struct WDC_API ConversationResponse : public ISerializable
{
    RTTI_DECL();

    std::vector<ConversationIntent> m_Intents;
    std::vector<ConversationEntities> m_Entities;
    std::vector<std::string> m_Output;
    std::string m_IntentOverrideTag;
    Json::Value m_Context;

    virtual void Serialize(Json::Value & json)
    {
        json["context"] = m_Context;
        SerializeVector("intents", m_Intents, json);
        SerializeVector("text", m_Output, json["output"]);
        SerializeVector("entities", m_Entities, json);
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("intents"))
            DeserializeVector("intents", json, m_Intents);

        if( json.isMember("output") && json["output"].isMember("text") )
            DeserializeVector("text", json["output"], m_Output);

        if( json.isMember("context") ) {
            m_Context = json["context"];

            // Intent Override
            // Looks for an intent override tag and if one exists and is in the context
            // it will use that as the intent opposed to the intent from Conversation
            if (json.isMember("input") && json["input"].isMember("intentoverride")
                    && json["input"]["intentoverride"] != "" ) {
                m_IntentOverrideTag = json["input"]["intentoverride"].asString();
                if (m_Context.isMember(m_IntentOverrideTag) && m_Context[m_IntentOverrideTag] != ""
                        && m_Intents.size() > 0 )
                    m_Intents[0].m_Intent = m_Context[m_IntentOverrideTag].asString();
            }
        }

        if( json.isMember("entities") )
            DeserializeVector("entities", json, m_Entities);
    }
};

#endif
