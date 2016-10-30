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

#ifndef SELF_NAMEGREETERAGENT_H
#define SELF_NAMEGREETERAGENT_H

#include "agent/IAgent.h"
#include "blackboard/Person.h"
#include "blackboard/Image.h"
#include "blackboard/NameIntent.h"
#include "services/FacialRecognition.h"
#include "utils/TimerPool.h"


class FaceAgent : public IAgent
{
public:
    RTTI_DECL();

    FaceAgent() : m_SaySomething(false),
		m_WaitTime(30.0f),
		m_IsTraining(false), 
		m_MinConfidence(0.0),
		m_PersonInterval(300.0f),
		m_SessionInterval(0.0),
		m_pPeople(NULL),
		m_pFacialRecognition(NULL)
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value &json);
    virtual void Deserialize(const Json::Value &json);

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:

    bool                        m_SaySomething;
    bool                        m_IsTraining;
    std::string                 m_FaceId;
    std::string                 m_Session;
    std::string                 m_LearnedName;
    std::string                 m_NewFace;
    std::string                 m_RememberedFace;
    std::vector<std::string>    m_Sessions;
    std::vector<std::string>    m_Greetings;
    std::vector<std::string>    m_GenericGreetings;
    People *                    m_pPeople;
    double                      m_MinConfidence;
    float                       m_PersonInterval;
    float                       m_SessionInterval;
    float		                m_WaitTime;
    TimerPool::ITimer::SP       m_spWaitTimer;
    TimerPool::ITimer::SP       m_spPeopleTimer;
    TimerPool::ITimer::SP       m_spSessionTimer;
    Person::SP                  m_spPerson;
    FacialRecognition *         m_pFacialRecognition;

    void		                OnPerson(const ThingEvent &a_ThingEvent);
    void                        OnLearnFace(const ThingEvent &a_ThingEvent);
    void		                OnEnableGreeting();
    void                        GetPeople();
    void                        OnGetPeople(People * a_pPeople);
    void                        OnDetectFaces(Detect * a_Detect);
    void                        OnAddedPerson(const Json::Value & a_Callback);
    void                        OnAddedFace(const Json::Value & a_Callback);
    void                        OnTrained(Trainer * a_Trainer);
    void                        GetSession();
    void                        OnSession(Session * a_Session);




};

#endif //SELF_NAMEGREETERAGENT_H
