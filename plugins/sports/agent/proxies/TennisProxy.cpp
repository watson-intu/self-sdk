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

#include "TennisProxy.h"
#include "SelfInstance.h"
#include "services/IService.h"

REG_SERIALIZABLE( TennisProxy );
RTTI_IMPL( TennisProxy, IQuestionAnswerProxy);


TennisProxy::TennisProxy()
{}

//! ISerializable
void TennisProxy::Serialize(Json::Value & json)
{}

void TennisProxy::Deserialize(const Json::Value & json)
{}

//! IQuestionAnswer
void TennisProxy::AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback)
{
    std::string temp = a_spQuestion->GetText();
    new TennisProxy::TennisRequest( this, temp, a_Callback );
}

void TennisProxy::Start()
{
    Log::Debug("TennisProxy", "Started...");
    m_pTennis = SelfInstance::GetInstance()->FindService<Tennis>();
}

void TennisProxy::Stop()
{

}

//! TennisProxy Specific

//! TennisRequest helper object
TennisProxy::TennisRequest::TennisRequest( TennisProxy * a_pTennisProxy, std::string a_Question,
                                     Delegate<const Json::Value &> a_Callback ) :
        m_pTennisProxy( a_pTennisProxy ),
        m_Question( a_Question ),
        m_Callback( a_Callback )
{
    SendRequest();
}

void TennisProxy::TennisRequest::SendRequest()
{
    if ( m_pTennisProxy->m_pTennis ) {
        m_pTennisProxy->m_pTennis->SendRequest(m_Question,
                                           DELEGATE( TennisRequest, OnResponse, const Json::Value &, this ));
    }
    else
    {
        Log::Error("TennisProxy", "TennisService not configured");
        delete this;
    }
}

void TennisProxy::TennisRequest::OnResponse( const Json::Value & json )
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