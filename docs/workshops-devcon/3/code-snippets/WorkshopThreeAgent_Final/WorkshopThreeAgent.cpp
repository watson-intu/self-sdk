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

#include "WorkshopThreeAgent.h"
#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Goal.h"
#include "skills/SkillManager.h"

REG_SERIALIZABLE( WorkshopThreeAgent );
RTTI_IMPL( WorkshopThreeAgent, IAgent);

//  Serialisation() takes an object created and turns it back into a JSON object for it to be published.
void WorkshopThreeAgent::Serialize(Json::Value & json)
{
    IAgent::Serialize( json );

    SerializeVector("m_NegativeTones", m_NegativeTones, json);
    SerializeVector("m_PositiveTones", m_PositiveTones, json);

    json["m_EmotionalState"] = m_EmotionalState;
    json["m_EmotionTime"] = m_EmotionTime;
}

//  Deserialize will read in JSON data and break it down into an object that can be read and used by the Agent.
void WorkshopThreeAgent::Deserialize(const Json::Value & json)
{
    IAgent::Deserialize( json );

    DeserializeVector("m_NegativeTones", json, m_NegativeTones);
    DeserializeVector("m_PositiveTones", json, m_PositiveTones);

    if( json.isMember( "m_EmotionalState" ) )
        m_EmotionalState = json["m_EmotionalState"].asFloat();
    if (json.isMember("m_EmotionTime"))
        m_EmotionTime = json["m_EmotionTime"].asFloat();

    if (m_NegativeTones.size() == 0)
        m_NegativeTones.push_back("extraversion");
    if (m_PositiveTones.size() == 0)
        m_PositiveTones.push_back("joy");
}

//  OnStart() will subscribe the Emotion Agent to start listening for updates from the Blackboard and start listening.
//  OnStart() will initialise OnEmtion, OnLearningIntent, OnText and OnEmotionCheck listening for any updates which
// are posted to the Blackboard.
bool WorkshopThreeAgent::OnStart()
{
    SelfInstance * pInstance = SelfInstance::GetInstance();
    assert( pInstance != NULL );
    BlackBoard * pBlackboard = pInstance->GetBlackBoard();
    assert( pBlackboard != NULL );

    pBlackboard->SubscribeToType( Emotion::GetStaticRTTI().GetName(),
                                  DELEGATE( WorkshopThreeAgent, OnEmotion, const ThingEvent &, this ), TE_ADDED );
    pBlackboard->SubscribeToType(LearningIntent::GetStaticRTTI().GetName(),
                                 DELEGATE(WorkshopThreeAgent, OnLearningIntent, const ThingEvent &, this), TE_ADDED);
    pBlackboard->SubscribeToType(Text::GetStaticRTTI().GetName(),
                                 DELEGATE(WorkshopThreeAgent, OnText, const ThingEvent &, this), TE_ADDED);

    m_spEmotionTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(WorkshopThreeAgent, OnEmotionCheck, this), m_EmotionTime, true, true);
    return true;
}

//  OnStop() will unsubscribe the Emotion Agent, stopping it from listening for updates posted to the Blackboard.
//  This will ensure processes are won't be running in the background.
bool WorkshopThreeAgent::OnStop()
{
    SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType( Emotion::GetStaticRTTI(), this );
    SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType(LearningIntent::GetStaticRTTI(), this);
    return true;
}

//  OnEmotion will listen to the Blackboard for objects being posted .
void WorkshopThreeAgent::OnEmotion( const ThingEvent & a_ThingEvent )
{
    Log::Debug("WorkshopThreeAgent", "Emotion object addition to the blackboard detected");
    Emotion::SP spEmotion = DynamicCast<Emotion>( a_ThingEvent.GetIThing() );
    if(spEmotion)
    {
        SelfInstance::GetInstance()->GetSkillManager()->UseSkill( spEmotion->GetType(), spEmotion );
    }
}

//  OnText() waits for a text response from Tone Analyser which listens for OnTone()?
void WorkshopThreeAgent::OnText(const ThingEvent & a_ThingEvent)
{
    Text::SP spText = DynamicCast<Text>(a_ThingEvent.GetIThing());
    if (spText)
    {
        ToneAnalyzer * tone = SelfInstance::GetInstance()->FindService<ToneAnalyzer>();
        if (tone != NULL)
        {
            tone->GetTone(spText->GetText(), DELEGATE(WorkshopThreeAgent, OnTone, DocumentTones *, this));
        }
    }
}

//  OnLearningIntent() waits for a response waits for a target response, and depending on feedback being positive or net
//  and emotional state being constrained between zero and one, increments the emotional state. The updated emotional state
//  is then published to the Blackboard.
void WorkshopThreeAgent::OnLearningIntent(const ThingEvent & a_ThingEvent)
{
    LearningIntent::SP spLearningIntent = DynamicCast<LearningIntent>(a_ThingEvent.GetIThing());
    if (spLearningIntent && spLearningIntent->GetVerb().compare("feedback") == 0)
    {
        if (spLearningIntent->GetTarget().compare("positive_feedback") == 0)
        {
            if(m_EmotionalState < 1.0)
                m_EmotionalState += 0.1f;
        }
        else
        {
            if (m_EmotionalState > 0.0)
                m_EmotionalState -= 0.1f;
        }

        PublishEmotionalState();
    }
}

//  OnTone() waits for the response from the Tone Analyser. OnTone() will then pick out the top scored value for the tone.
// Once updated this will be published to the Blackboard.
void WorkshopThreeAgent::OnTone(DocumentTones * a_Callback)
{
    if (a_Callback != NULL)
    {
        double topScore = 0.0;
        Tone tone;
        for (size_t i = 0; i < a_Callback->m_ToneCategories.size(); ++i)
        {
            for (size_t j = 0; j < a_Callback->m_ToneCategories[i].m_Tones.size(); ++j)
            {
                Tone someTone = a_Callback->m_ToneCategories[i].m_Tones[j];
                if (someTone.m_Score > topScore)
                {
                    topScore = someTone.m_Score;
                    tone = someTone;
                }
            }
        }
        Log::Debug("WorkshopThreeAgent", "Found top tone as: %s", tone.m_ToneName.c_str());
        bool toneFound = false;
        for (size_t i = 0; i < m_PositiveTones.size(); ++i)
        {
            if (tone.m_ToneId == m_PositiveTones[i])
            {
                toneFound = true;
                if (m_EmotionalState < 1.0f)
                    m_EmotionalState += 0.1f;
            }
        }

        if (!toneFound)
        {
            for (size_t i = 0; i < m_NegativeTones.size(); ++i)
            {
                if (tone.m_ToneId == m_NegativeTones[i])
                {
                    if (m_EmotionalState > 0.0f)
                        m_EmotionalState -= 0.1f;
                }
            }
        }

        PublishEmotionalState();
    }
}

//  Every 30 seconds OnEmotionCheck() will either increase or decrease the EmotionalState value to ensure that over time
//  EmotionalState resolve back to a value of 0.5 over time.
void WorkshopThreeAgent::OnEmotionCheck()
{
    if (m_EmotionalState > 0.5)
        m_EmotionalState -= 0.1f;
    else
        m_EmotionalState += 0.1f;

    PublishEmotionalState();
}

//  PublishEmotionalState() will publish a value of EmotionalState to the Blackboard. Agents listening for the
//  EmotionalState variable will be updated when a new EmotionalState is published.
void WorkshopThreeAgent::PublishEmotionalState()
{
    Json::Value json;
    json["m_EmotionalState"] = m_EmotionalState;

    SelfInstance::GetInstance()->GetBlackBoard()->AddThing(IThing::SP(
            new IThing(TT_PERCEPTION, "ADDED", 3600.0f, "EmotionalState", json, 1.0f)));
}
