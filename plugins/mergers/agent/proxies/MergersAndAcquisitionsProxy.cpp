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

#include "MergersAndAcquisitionsProxy.h"
#include "SelfInstance.h"
#include "services/IService.h"

REG_SERIALIZABLE(MergersAndAcquisitionsProxy);
RTTI_IMPL(MergersAndAcquisitionsProxy, IQuestionAnswerProxy);


MergersAndAcquisitionsProxy::MergersAndAcquisitionsProxy()
{}

//! ISerializable
void MergersAndAcquisitionsProxy::Serialize(Json::Value & json)
{}

void MergersAndAcquisitionsProxy::Deserialize(const Json::Value & json)
{}

//! IQuestionAnswer
void MergersAndAcquisitionsProxy::AskQuestion(QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback)
{
	new MergersAndAcquisitionsProxy::MergersAndAcquisitionsRequest(this, a_spQuestion->GetText(), a_Callback);
}

void MergersAndAcquisitionsProxy::Start()
{
	Log::Debug("MergersAndAcquisitionsProxy", "Started...");
	m_pMergersAndAcquisitions = SelfInstance::GetInstance()->FindService<MergersAndAcquisitions>();
}

void MergersAndAcquisitionsProxy::Stop()
{
	Log::Debug("MergersAndAcquisitionsProxy", "Stopped...");
}

MergersAndAcquisitionsProxy::MergersAndAcquisitionsRequest::MergersAndAcquisitionsRequest(
	MergersAndAcquisitionsProxy * a_pMergersAndAcquisitions, const std::string & a_Question,
	Delegate<const Json::Value &> a_Callback) :
	m_pMergersAndAcquisitionsProxy(a_pMergersAndAcquisitions),
	m_Question(a_Question),
	m_Callback(a_Callback)
{
	if (m_pMergersAndAcquisitionsProxy->m_pMergersAndAcquisitions != NULL ) 
	{
		m_pMergersAndAcquisitionsProxy->m_pMergersAndAcquisitions->SendRequest(m_Question,
			DELEGATE(MergersAndAcquisitionsRequest, OnResponse, const std::string &, this));
	}
	else
	{
		Log::Error("MergersAndAcquisitionsProxy", "MergersAndAcquisitions Service not configured");
		if ( m_Callback.IsValid() )
			m_Callback( Json::Value() );
		delete this;
	}
}

void MergersAndAcquisitionsProxy::MergersAndAcquisitionsRequest::OnResponse(const std::string & json)
{
	Json::Value answer;
	if (json.size() > 0)
	{
		Json::Value root;

		Json::Reader reader(Json::Features::strictMode());
		if (reader.parse(json, root))
		{
			answer["response"] = root;
			answer["confidence"] = answer["response"][0]["scores"]["aggregate"].asDouble();
			Log::Debug("MergersAndAcquisitionsProxy", "Received the following payload from service: %s",
				answer.toStyledString().c_str());
		}
	}
	else
		Log::Error("MergersAndAcquisitionsProxy", "Failed to get valid response");

	if (m_Callback.IsValid())
		m_Callback(answer);
	delete this;
}