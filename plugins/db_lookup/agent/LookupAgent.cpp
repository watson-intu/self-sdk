/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#include "LookupAgent.h"
#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Goal.h"
#include "blackboard/WebRequest.h"
#include "skills/SkillManager.h"

REG_SERIALIZABLE(LookupAgent);
RTTI_IMPL(LookupAgent, IAgent);

LookupAgent::LookupAgent()
{}

void LookupAgent::Serialize(Json::Value & json)
{
	IAgent::Serialize(json);

	json["m_KeyPrefix"] = m_KeyPrefix;
	json["m_URL"] = m_URL;

	int index = 0;
	for (Headers::iterator iHeader = m_Headers.begin(); iHeader != m_Headers.end(); ++iHeader)
	{
		json["m_Headers"][index]["key"] = iHeader->first;
		json["m_Headers"][index++]["value"] = iHeader->second;
	}
}

void LookupAgent::Deserialize(const Json::Value & json)
{
	IAgent::Deserialize(json);

	if (json.isMember("m_KeyPrefix"))
		m_KeyPrefix = json["m_KeyPrefix"].asString();
	if (json.isMember("m_URL"))
		m_URL = json["m_URL"].asString();

	m_Headers.clear();
	if (json.isMember("m_Headers"))
	{
		const Json::Value & headers = json["m_Headers"];
		for (Json::ValueConstIterator iHeader = headers.begin(); iHeader != headers.end(); ++iHeader)
			m_Headers[(*iHeader)["key"].asString()] = (*iHeader)["value"].asString();
	}
}

bool LookupAgent::OnStart()
{
	Log::Debug("LookupAgent", "Lookup Agent has started!");
	SelfInstance::GetInstance()->GetBlackBoard()->SubscribeToType(LookupIntent::GetStaticRTTI(),
		DELEGATE(LookupAgent, OnLookupIntent, const ThingEvent &, this), TE_ADDED);

	return true;
}

bool LookupAgent::OnStop()
{
	Log::Debug("LookupAgent", "Lookup Agent has stopped!");
	SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType(LookupIntent::GetStaticRTTI(), this);
	return true;
}

void LookupAgent::OnLookupIntent(const ThingEvent & a_ThingEvent)
{
	LookupIntent::SP spLookup = DynamicCast<LookupIntent>(a_ThingEvent.GetIThing());
	if (spLookup)
	{
		if (!m_spActive)
		{
			ExecuteRequest(spLookup);
		}
		else
		{
			// r&r request already active, just push into the queue
			m_LookupRequests.push_back(spLookup);
		}
	}
}

void LookupAgent::ExecuteRequest(LookupIntent::SP a_pLookup)
{
	Log::Debug("LookupAgent", "Lookup Intent object added to blackboard detected");

	SelfInstance * pInstance = SelfInstance::GetInstance();

	m_spActive = a_pLookup;
	m_spActive->SetState("PROCESSING");
	std::string key = a_pLookup->GetIntent();
	if (key.compare(0, m_KeyPrefix.length(), m_KeyPrefix) == 0)
		key = key.erase(0, m_KeyPrefix.length()) + "_";
	else
		key += "_";
	key += a_pLookup->GetEntity();
	std::vector<std::string> answers;
	StringUtil::Split(a_pLookup->GetCollection(), ",", answers);
	if (answers.size() != 2)
	{
		Log::Error("LookupAgent", "Invalid schema - expecting clusterId,WorkspaceName from Conversation Service!");
		a_pLookup->SetState("FAILED");
	}
	else
	{
		RetrieveAndRank * pRR = pInstance->FindService<RetrieveAndRank>();
		if (pRR != NULL)
		{
			pRR->Select(answers[0], answers[1], key, 
				DELEGATE(LookupAgent, OnMessage, RetrieveAndRankResponse *, this));
		}
		else
		{
			Log::Error("LookupAgent", "RetrieveAndRank service not configured correctly - will not proceed");
			m_spActive->SetState("FAILED");
		}
	}

}

void LookupAgent::OnMessage(RetrieveAndRankResponse * a_pRetrieveAndRankResponse)
{
	bool bFailed = true;
	if (a_pRetrieveAndRankResponse != NULL)
	{
		std::vector<Documents> m_Documents = a_pRetrieveAndRankResponse->m_Docs;
		if (m_Documents.size() > 0)
		{
			std::string m_Body = StringUtil::Trim(m_Documents[0].m_Body);
			size_t i = 0;
			while (i < m_Body.length())
			{
				if (m_Body[i] != '{')
					++i;
				else
					break;
			}
			m_Body = m_Body.erase(0, i);

			Json::Value json;
			Json::Reader reader;
			bool parsingSuccessful = reader.parse(m_Body.c_str(), json);
			if (parsingSuccessful)
			{
				std::string db = json["dbType"].asString();
				std::string query = json["query"].asString();
				std::string params = "/GetData?dbType=" + db;
				params += "&query=" + StringUtil::UrlEscape(query);

				m_spActive->AddChild(WebRequest::SP(new WebRequest(m_URL, "GET", "", params, m_Headers)));
				m_spActive->SetState("COMPLETED");
				bFailed = false;
			}
		}
	}

	if ( bFailed )
	{
		Log::Error("LookupAgent", "Failed to process LookupIntent." );
		m_spActive->SetState("FAILED");
	}

	if (m_LookupRequests.begin() != m_LookupRequests.end())
	{
		LookupIntent::SP spLookup = m_LookupRequests.front();
		m_LookupRequests.pop_front();

		ExecuteRequest(spLookup);
	}
	else
	{
		m_spActive.reset();
	}
}