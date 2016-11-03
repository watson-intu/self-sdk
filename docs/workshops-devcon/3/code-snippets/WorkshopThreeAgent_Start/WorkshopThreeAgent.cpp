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

// Serialize will build a JSON object from the object data created by the Emotion Agent.
void WorkshopThreeAgent::Serialize(Json::Value & json)
{
    IAgent::Serialize( json );

    SerializeVector("m_NegativeTones", m_NegativeTones, json);
    SerializeVector("m_PositiveTones", m_PositiveTones, json);

    json["m_EmotionalState"] = m_EmotionalState;
    json["m_EmotionTime"] = m_EmotionTime;
}

//  Read in JSON data and create a readable object for functions to read from.
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

//  OnEmotion will listen to the Blackboard for objects being posted.
void WorkshopThreeAgent::OnEmotion( const ThingEvent & a_ThingEvent )
{
    Log::Debug("EmotionAgent", "Emotion object addition to the blackboard detected");
    Emotion::SP spEmotion = DynamicCast<Emotion>( a_ThingEvent.GetIThing() );
    if(spEmotion)
    {
        SelfInstance::GetInstance()->GetSkillManager()->UseSkill( spEmotion->GetType(), spEmotion );
    }
}

//  OnText() waits for a text repsponse Tone Analyser instance
void WorkshopThreeAgent::OnText(const ThingEvent & a_ThingEvent)
{

}

//  OnLearningIntent() waits for a response waits for a target response, and depending on feedback being positive or net
//  and emotional state being constrained between zero and one, increments the emotional state. The updated emotional state
//  is then published to the Blackboard.
void WorkshopThreeAgent::OnLearningIntent(const ThingEvent & a_ThingEvent)
{

}

//  OnTone() waits for the response from the Tone Analyser...
void WorkshopThreeAgent::OnTone(DocumentTones * a_Callback)
{

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

//  PublishEmotionalState() will publish a value of EmotionalState to the Blackboard. Agents listening for EmotionalState
//  will be updated when a new EmotionalState is published.
void WorkshopThreeAgent::PublishEmotionalState()
{
    Json::Value json;
    json["m_EmotionalState"] = m_EmotionalState;

    SelfInstance::GetInstance()->GetBlackBoard()->AddThing(IThing::SP(
            new IThing(TT_PERCEPTION, "ADDED", 3600.0f, "EmotionalState", json, 1.0f)));
}
