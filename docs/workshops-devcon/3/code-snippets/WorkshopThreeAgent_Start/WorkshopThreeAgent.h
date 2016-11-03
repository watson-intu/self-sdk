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

#ifndef SELF_WORKSHOP_THREE_AGENT_AGENT_H
#define SELF_WORKSHOP_THREE_AGENT_AGENT_H

#include "agent/IAgent.h"
#include "blackboard/Emotion.h"
#include "blackboard/Text.h"
#include "blackboard/LearningIntent.h"
#include "services/WeatherInsights.h"
#include "services/ToneAnalyzer/ToneAnalyzer.h"
#include "sensors/SensorManager.h"
#include "sensors/MoodData.h"
#include "SelfLib.h"

class SkillInstance;

//! This agent handles emotions

class WorkshopThreeAgent : public IAgent
{
public:
    RTTI_DECL();

    WorkshopThreeAgent() : m_EmotionalState( 0.5f ), m_EmotionTime ( 30.0f )
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Data
    std::vector<std::string>	m_NegativeTones;
    std::vector<std::string>	m_PositiveTones;
    float						m_EmotionalState;
    float						m_EmotionTime;
    TimerPool::ITimer::SP		m_spEmotionTimer;

    //! Event Handlers
    void 		OnEmotion(const ThingEvent & a_ThingEvent);
    void		OnText(const ThingEvent & a_ThingEvent);
    void		OnLearningIntent(const ThingEvent & a_ThingEvent);
    void		OnTone(DocumentTones * a_Callback);

    void		OnEnableEmotion();
    void		OnEmotionCheck();
    void		PublishEmotionalState();
};

#endif // SELF_WORKSHOP_THREE_AGENT_AGENT_H

