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

#ifndef SELF_LEARNINGAGENT_H
#define SELF_LEARNINGAGENT_H

#include "IAgent.h"
#include "blackboard/LearningIntent.h"
#include "blackboard/Text.h"
#include "blackboard/Image.h"
#include "blackboard/Confirm.h"
#include "blackboard/Entity.h"
#include "blackboard/UsedSkill.h"
#include "blackboard/Goal.h"
#include "utils/Factory.h"

#include "SelfLib.h"


class SELF_API LearningAgent : public IAgent
{
public:
	RTTI_DECL();

	LearningAgent();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Callbacks
	void OnGoal(const ThingEvent & a_ThingEvent);
	void OnSkillUsed( const ThingEvent & a_ThingEvent );
	void OnText(const ThingEvent & a_ThingEvent);
	void OnImage(const ThingEvent & a_ThingEvent);
	void OnLearningIntent(const ThingEvent & a_ThingEvent);
	void OnLearnSkill( LearningIntent::SP a_spIntent );
	void OnForgetSkill( LearningIntent::SP a_spIntent );
	void OnRetrainNLC(LearningIntent::SP a_spIntent, 
		const std::string & a_Class );
	void OnConfirmRetrain(Confirm::SP a_spConfirm);
	void OnLearnObject( LearningIntent::SP a_spIntent );
	void OnForgetObject( LearningIntent::SP a_spIntent );

	//! Data
	Goal::SP					m_spLastFailedGoal;
	std::list< UsedSkill::SP >	m_UsedSkills;

	std::vector<std::string>	m_LearningResponses;
	std::vector<std::string>	m_ForgetResponses;
	std::vector<std::string>	m_ConfirmRetrain;
	std::vector<std::string>	m_RetrainResponses;
	std::vector<std::string>	m_LearnObjectResponses;
	std::vector<std::string>	m_ForgetObjectResponses;
	std::vector<std::string>    m_FailedImageTraining;
	std::list<Text::SP>			m_TextHistory;
	std::list<Image::SP>		m_ImageHistory;
	bool						m_bEnableSkillTraining;
	bool						m_bEnableNLCTraining;
	bool						m_bEnableObjectTraining;
};

#endif //SELF_LEARNINGAGENT_H
