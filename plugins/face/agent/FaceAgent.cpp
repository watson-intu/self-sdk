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

#include "FaceAgent.h"
#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/IThing.h"
#include "blackboard/Say.h"

REG_SERIALIZABLE(FaceAgent);
RTTI_IMPL( FaceAgent, IAgent);


void FaceAgent::Serialize(Json::Value & json)
{
    IAgent::Serialize(json);
    SerializeVector("m_Greetings", m_Greetings, json);
    SerializeVector("m_GenericGreeting", m_GenericGreetings, json);
    json["m_MinConfidence"] = m_MinConfidence;
    json["m_RememberedFace"] = m_RememberedFace;
    json["m_NewFace"] = m_NewFace;
    json["m_PersonInterval"] = m_PersonInterval;
    json["m_SessionInterval"] = m_SessionInterval;

}

void FaceAgent::Deserialize(const Json::Value & json)
{
    IAgent::Deserialize(json);
    DeserializeVector("m_Greetings", json, m_Greetings);
    DeserializeVector("m_GenericGreeting", json, m_GenericGreetings);

    if (json.isMember("m_MinConfidence"))
        m_MinConfidence = json["m_MinConfidence"].asDouble();
    if (json.isMember("m_NewFace"))
        m_NewFace = json["m_NewFace"].asString();
    if (json.isMember("m_RememberedFace"))
        m_RememberedFace = json["m_RememberedFace"].asString();
    if (json.isMember("m_PersonInterval"))
        m_PersonInterval = json["m_PersonInterval"].asFloat();
    if (json.isMember("m_SessionInterval"))
        m_SessionInterval = json["m_SessionInterval"].asFloat();
}

bool FaceAgent::OnStart()
{
    SelfInstance::GetInstance()->GetBlackBoard()->SubscribeToType(Person::GetStaticRTTI(),
                                                                  DELEGATE( FaceAgent, OnPerson, const ThingEvent &, this ), TE_STATE);
    SelfInstance::GetInstance()->GetBlackBoard()->SubscribeToType(NameIntent::GetStaticRTTI(),
                                                                  DELEGATE(FaceAgent, OnLearnFace, const ThingEvent &, this), TE_ADDED);
    m_pFacialRecognition = SelfInstance::GetInstance()->FindService<FacialRecognition>();
    if ( m_pFacialRecognition == NULL )
    {
        Log::Error("FaceAgent", "FaceRecognition service not available.");
    }
    GetPeople();
    m_spWaitTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(FaceAgent, OnEnableGreeting, this), m_WaitTime, true, true);

	return true;
}

bool FaceAgent::OnStop()
{
    m_spWaitTimer.reset();
    m_spPeopleTimer.reset();
    m_spSessionTimer.reset();

    SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType(Person::GetStaticRTTI(), this);
    SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType(NameIntent::GetStaticRTTI(), this);

    delete m_pPeople;

	return true;
}

void FaceAgent::GetPeople()
{
    Log::Status("FaceAgent", "Getting People...");
    if ( m_pFacialRecognition != NULL )
        m_pFacialRecognition->GetAllPeople(DELEGATE(FaceAgent, OnGetPeople, People *, this));
    else
        Log::Error("FaceAgent", "FaceRecognition is NULL.");

    // start a timer to keep checking people on a regular interval
    if (!m_spPeopleTimer)
    {
        m_spPeopleTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(FaceAgent, GetPeople, this),
                                                            m_PersonInterval, true, true);
    }
}

void FaceAgent::OnGetPeople(People * a_pPeople)
{
	if ( a_pPeople != NULL )
	{
		Log::Status("FaceAgent", "Received new list of people for Facial Recognition");
		if(m_pPeople != NULL)
			delete m_pPeople;
		m_pPeople = a_pPeople;
		for (size_t i = 0; i < m_pPeople->m_People.size(); ++i)
		{
			const Face &f = m_pPeople->m_People[i];
			Log::Debug("FaceAgent", "Person: %s", f.m_Name.c_str());
		}
	}
	else
		Log::Error( "FaceAgent", "OnGetPeople() failed." );
}

void FaceAgent::OnPerson(const ThingEvent & a_ThingEvent)
{
    m_spPerson = DynamicCast<Person>( a_ThingEvent.GetIThing() );
    if(!m_spPerson->GetState().compare("named"))
    {
        if (m_SaySomething) 
		{
            Log::Debug("FaceAgent", "Found Person %s with confidence %d", m_spPerson->GetName().c_str(),
                       m_spPerson->GetConfidence());
            if (m_spPerson->GetConfidence() > m_MinConfidence) 
			{
                std::string greeting = m_Greetings[rand() % m_Greetings.size()];
                SelfInstance::GetInstance()->GetBlackBoard()->AddThing(
                        Say::SP(new Say(StringUtil::Format(greeting.c_str(), m_spPerson->GetName().c_str()))));
            }
            else {
                std::string greeting = m_GenericGreetings[rand() % m_GenericGreetings.size()];
                SelfInstance::GetInstance()->GetBlackBoard()->AddThing(
                        Say::SP(new Say(greeting.c_str())));
            }
            m_SaySomething = false;
        }
    }
}

void FaceAgent::OnLearnFace(const ThingEvent & a_ThingEvent)
{
    Image::SP spImage = DynamicCast<Image>(m_spPerson->GetParent());
    NameIntent::SP spNameIntent = DynamicCast<NameIntent>( a_ThingEvent.GetIThing() );
    m_LearnedName = spNameIntent->GetName();
    m_pFacialRecognition->DetectFace(spImage->GetContent(), DELEGATE(FaceAgent, OnDetectFaces, Detect *, this) );
}

void FaceAgent::OnDetectFaces(Detect * a_Detect)
{
    // Check if name exist in current People Object
    bool personFound = false;
    m_FaceId = a_Detect->m_FaceId;
    for (size_t i = 0; i < m_pPeople->m_People.size(); ++i)
    {
        const Face & f = m_pPeople->m_People[i];
        Log::Debug("FaceAgent", "Looking for match between %s and %s", f.m_Name.c_str(), m_LearnedName.c_str());
        if (f.m_Name == m_LearnedName)
        {
            personFound = true;
            break;
        }
    }

    if(!personFound)
    {
        SelfInstance::GetInstance()->GetBlackBoard()->AddThing(
                Say::SP(new Say(StringUtil::Format(m_NewFace.c_str(), m_LearnedName.c_str()))));
        m_pFacialRecognition->AddPerson(m_LearnedName,
                                      DELEGATE(FaceAgent, OnAddedPerson, const Json::Value &, this) );
    }
    else
    {
        SelfInstance::GetInstance()->GetBlackBoard()->AddThing(
                Say::SP(new Say(StringUtil::Format(m_RememberedFace.c_str(), m_LearnedName.c_str()))));
        m_pFacialRecognition->AddFace(m_LearnedName, m_FaceId,
                                    DELEGATE(FaceAgent, OnAddedFace, const Json::Value &, this));
    }
    delete a_Detect;
}

void FaceAgent::OnAddedPerson(const Json::Value & a_Callback)
{
    m_pFacialRecognition->AddFace(m_LearnedName, m_FaceId,
                                DELEGATE(FaceAgent, OnAddedFace, const Json::Value &, this) );
}

void FaceAgent::OnAddedFace(const Json::Value & a_Callback)
{
    if(m_IsTraining)
    {
        m_Sessions.push_back(m_Session);
    }
    else
    {
        m_pFacialRecognition->TrainIdentifier(DELEGATE(FaceAgent, OnTrained, Trainer * , this));
    }
}

void FaceAgent::OnTrained(Trainer * a_Trainer)
{
    m_IsTraining = true;
    m_Session = a_Trainer->m_Session;
    m_spSessionTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(FaceAgent, GetSession, this),
                                                         m_SessionInterval, true, true);
    delete a_Trainer;
}

void FaceAgent::GetSession()
{
    Log::Debug("FaceAgent", "Received Session ID: %s", m_Session.c_str());
    m_pFacialRecognition->GetSessionStatus(m_Session,
                                           DELEGATE(FaceAgent, OnSession, Session * , this));
}

void FaceAgent::OnSession(Session * a_Session)
{
	if ( a_Session != NULL )
	{
		Log::Debug("FaceAgent", "On Session Response Received: %s", a_Session->m_Status.c_str());
		if(a_Session->m_Status == "SUCC")
		{
			m_spSessionTimer.reset();
			if(m_Sessions.size() > 0)
			{
				m_Sessions.pop_back();
				m_pFacialRecognition->TrainIdentifier(DELEGATE(FaceAgent, OnTrained, Trainer * , this));
			}
			else
			{
				m_IsTraining = false;
			}
		}
		delete a_Session;
	}
	else
	{
		Log::Error( "FaceAgent", "OnSession() failure." );
	}
}

void FaceAgent::OnEnableGreeting()
{
    Log::Debug("FaceAgent", "Timer fired, greeting enabled.");
    m_SaySomething = true;
}


