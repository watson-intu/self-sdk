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

#include "Motion.h"

REG_SERIALIZABLE(Motion);
RTTI_IMPL( Motion, IThing);

void Motion::Serialize(Json::Value &json)
{
    IThing::Serialize(json);

}

void Motion::Deserialize(const Json::Value &json)
{
    IThing::Deserialize(json);

}

bool Motion::Create(const Json::Value & a_Response) {
    Log::Debug("Motion", "Created Motion Object");
    Log::Debug("Motion", "Received value: %s", Json::FastWriter().write(a_Response).c_str());
	std::string response = Json::FastWriter().write(a_Response);
	if (Json::FastWriter().write(a_Response).find("in") != std::string::npos)
	{
		Log::Debug("Motion", "NOT ACTIVE!!");
		m_Active = false;
		Log::Debug("Motion", "m_Active is: %d", m_Active);
	}
	else
	{
		m_Active = true;
	}
	return true;

    /*m_Text = a_Active["text"].asString();

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
    }*/
}

