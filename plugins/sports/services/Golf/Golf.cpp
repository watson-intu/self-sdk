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

#include "Golf.h"
#include "SelfInstance.h"

REG_SERIALIZABLE( Golf );
RTTI_IMPL( Golf, IService );


Golf::Golf() : IService("GolfV1")
{}

//! ISerializable
void Golf::Serialize(Json::Value & json)
{
    IService::Serialize(json);
}

void Golf::Deserialize(const Json::Value & json)
{
    IService::Deserialize(json);
}

//! IService interface
bool Golf::Start()
{
    if (!IService::Start())
        return false;

    Log::Debug("Golf", "Url loaded as %s", m_pConfig->m_URL.c_str() );

    return true;
}

void Golf::SendRequest(const std::string & a_Input, 
    Delegate<const Json::Value &> a_Callback)
{
    std::string question = a_Input;
    std::replace( question.begin(), question.end(), ' ', '+');
    std::string url = m_pConfig->m_URL + question;    
    Log::Debug("Golf", "Firing at %s", url.c_str() );
    new RequestJson(url, "GET", NULL_HEADERS, EMPTY_STRING, 
        a_Callback, m_RequestTimeout);
}
