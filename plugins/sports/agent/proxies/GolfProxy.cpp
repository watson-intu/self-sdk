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

#include "GolfProxy.h"
#include "SelfInstance.h"
#include "services/IService.h"

REG_SERIALIZABLE( GolfProxy );
RTTI_IMPL( GolfProxy, IQuestionAnswerProxy );


GolfProxy::GolfProxy()
{}

//! ISerializable
void GolfProxy::Serialize(Json::Value & json)
{}

void GolfProxy::Deserialize(const Json::Value & json)
{}

//! IQuestionAnswer
void GolfProxy::AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback)
{
    std::string temp = a_spQuestion->GetText();    
    new GolfProxy::GolfRequest( this, temp, a_Callback );
}

void GolfProxy::Start()
{
    Log::Debug("GolfProxy", "Started...");
    m_pGolf = SelfInstance::GetInstance()->FindService<Golf>();
}

void GolfProxy::Stop()
{
	Log::Debug("GolfProxy", "Stopped...");
}

//! GolfProxy Specific

//! GolfRequest helper object
GolfProxy::GolfRequest::GolfRequest( GolfProxy * a_pGolfProxy, std::string a_Question, 
    Delegate<const Json::Value &> a_Callback ) :
    m_pGolfProxy( a_pGolfProxy ),
    m_Question( a_Question ),
    m_Callback( a_Callback )
{
    SendRequest();
}

void GolfProxy::GolfRequest::SendRequest()
{
    if ( m_pGolfProxy->m_pGolf ) {
        m_pGolfProxy->m_pGolf->SendRequest(m_Question, 
            DELEGATE( GolfRequest, OnResponse, const Json::Value &, this ));
    }
    else
    {
        Log::Error("GolfProxy", "Golf Service not configured");
        delete this;
    }
}

void GolfProxy::GolfRequest::OnResponse( const Json::Value & json )
{
    if (! json.isNull() )
    {
        Json::Value temp = json[0];
        if ( temp.isMember("answer") )
        {
            temp["response"].append( temp["answer"] );
        }
        if ( m_Callback.IsValid() )
            m_Callback( temp );
    }
    delete this;
}