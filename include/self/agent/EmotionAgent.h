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

#ifndef SELF_EMOTION_AGENT_H
#define SELF_EMOTION_AGENT_H

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

class SELF_API EmotionAgent : public IAgent
{
public:
	RTTI_DECL();

	EmotionAgent() : m_SaySomething(false), m_WaitTime( 30.0f ), m_EmotionalState( 0.5f ), 
		m_WeatherWaitTime( 3600.0f ), m_EmotionTime( 30.0f ), m_LastEmotionalState( 0.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

	const float GetEmotionalState() const
	{
		return m_EmotionalState;
	}

private:

	//! Types
	typedef SensorManager::SensorList   SensorList;

	//! Data
	SensorList					m_MoodSensors;
	bool 						m_SaySomething;
	float 						m_WaitTime;
	float						m_WeatherWaitTime;
	float						m_EmotionalState;
	float						m_LastEmotionalState;
	float						m_EmotionTime;
	TimerPool::ITimer::SP		m_spWaitTimer;
	TimerPool::ITimer::SP		m_spWeatherTimer;
	TimerPool::ITimer::SP		m_spEmotionTimer;
	std::vector<std::string>	m_NegativeTones;
	std::vector<std::string>	m_PositiveTones;
	std::vector<std::string>	m_PositiveWeather;
	std::vector<std::string>	m_NegativeWeather;

	//! Event Handlers
	void 		OnEmotion(const ThingEvent & a_ThingEvent);
	void		OnText(const ThingEvent & a_ThingEvent);
	void		OnLearningIntent(const ThingEvent & a_ThingEvent);
	void		OnWeatherData(const Json::Value & json);
	void		OnTone(DocumentTones * a_Callback);

	void		OnAddMoodSensor( ISensor * a_pSensor );
	void		OnRemoveMoodSensor( ISensor * a_pSensor );
	void        OnMoodData(IData * a_pData);

	void		OnEnableEmotion();
	void		OnEnableWeather();
	void		OnEmotionCheck();
	void		PublishEmotionalState();
};

#endif // EMOTION Agent

