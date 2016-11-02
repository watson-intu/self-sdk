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

#ifndef SELF_WORKSHOP_THREE_AGENT_AGENT_H
#define SELF_WORKSHOP_THREE_AGENT_AGENT_H

#include "IAgent.h"
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

class SELF_API WorkshopThreeAgent : public IAgent
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

