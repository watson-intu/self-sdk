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

#ifndef SELF_DIALOG_AGENT_H
#define SELF_DIALOG_AGENT_H

#include "IAgent.h"
#include "blackboard/Say.h"
#include "blackboard/Text.h"
#include "blackboard/Emotion.h"
#include "services/SpeechToText/SpeechToText.h"
#include "services/LanguageTranslation/LanguageTranslation.h"
#include "sensors/TouchData.h"
#include "sensors/SensorManager.h"
#include "sensors/ISensor.h"
#include "utils/Time.h"
#include "SelfLib.h"

class SkillInstance;

//! This agent handles dialog with the users
class SELF_API DialogAgent : public IAgent
{
public:
	RTTI_DECL();

	DialogAgent();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Types
	typedef std::list<Say::SP>		DialogList;
	typedef std::list<Emotion::SP>	EmotionList;

	struct VolumeTuning : public ISerializable
	{
		RTTI_DECL();

		VolumeTuning()
		{}
		VolumeTuning( const std::string & a_Sensor, const std::string & a_Skill ) : m_Sensor( a_Sensor ), m_Skill( a_Skill )
		{}

		std::string		m_Sensor;
		std::string		m_Skill;

		virtual void Serialize(Json::Value & json)
		{
			json["m_Sensor"] = m_Sensor;
			json["m_Skill"] = m_Skill;
		}
		virtual void Deserialize(const Json::Value & json)
		{
			m_Sensor = json["m_Sensor"].asString();
			m_Skill = json["m_Skill"].asString();
		}
	};

	//! Data
	std::string                 m_SpeechSkill;
	std::string					m_Language;
	std::string					m_Gender;
	bool						m_bGenerateURLs;
	bool						m_bExtractURLs;

	std::vector<std::string>    m_Interruptions;
	std::vector<std::string>    m_InterruptionSensors;
	std::vector<std::string>    m_InterruptionResponses;

	SensorManager::SensorList   m_TouchSensors;
	std::vector<std::string>    m_TouchSensorsEngaged;
	Time                        m_LastSensorEngageTime;
	int                         m_MinInterruptionSensors;
	float                       m_fInterruptionSensorInterval;

	Time 						m_BumperLastTime;
	float 						m_BumperDebounceInterval;
	bool                        m_bVolumeTuningFunctionality;

	Say::SP	                    m_spActive;
	DialogList                  m_Dialogs;
	EmotionList                 m_Emotions;
	std::vector<VolumeTuning>   m_VolumeTunings;

	LanguageTranslation *		m_pTranslation;

	//! Event Handlers
	void		OnDialog( const ThingEvent &a_ThingEvent );
	void 		OnText( const ThingEvent & a_ThingEvent );
	void		ExecuteDialog( Say::SP a_spDialog );
	void		OnSkillState( SkillInstance * a_pSkill );
	void		OnTouchData( IData * data );
	void 		OnTranslate(Translations * a_pTranslations);

};

#endif // SELF_DIALOG_AGENT_H
