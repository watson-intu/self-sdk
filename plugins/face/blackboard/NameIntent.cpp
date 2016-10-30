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

#include "NameIntent.h"

REG_SERIALIZABLE(NameIntent);
RTTI_IMPL( NameIntent, IIntent);

NameIntent::NameIntent()
{}

void NameIntent::Serialize(Json::Value &json)
{
    IIntent::Serialize(json);

    json["m_Text"] = m_Text;
    json["m_Target"] = m_Target;
    json["m_Name"] = m_Name;
}

void NameIntent::Deserialize(const Json::Value &json)
{
    IIntent::Deserialize(json);

    m_Text = json["m_Text"].asString();
    m_Target = json["m_Target"].asString();
    m_Name = json["m_Name"].asString();
}

void NameIntent::Create(const Json::Value & a_Intent, const Json::Value & a_Parse) {
    Log::Debug("NameIntent", "Created NameIntent Object");
    m_Text = a_Intent["text"].asString();

    const std::string &top_class = a_Intent["top_class"].asString();
    const std::string &parse = a_Parse["doc"]["sents"]["sent"][0]["parse"]["text"].asString();




    if (top_class == "name_statement")
    {
        m_Target = top_class;
        const std::string &type = a_Parse["doc"]["entities"]["entity"][0]["type"].asString();
        if(type == "PERSON" && a_Parse["doc"]["entities"]["entity"].size() > 1)
        {
            m_Name = a_Parse["doc"]["entities"]["entity"][0]["mentref"][0]["text"].asString();
        }
        else
        {
            std::size_t found = m_Text.find_last_of(" ");
            m_Name = m_Text.substr(found+1);
        }
    }
}
