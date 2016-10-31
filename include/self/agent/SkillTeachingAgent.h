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

#ifndef SELF_SKILLTEACHINGAGENT_H
#define SELF_SKILLTEACHINGAGENT_H

#include "IAgent.h"
#include "blackboard/LearningIntent.h"
#include "blackboard/Text.h"
#include "blackboard/Confirm.h"
#include "blackboard/UsedSkill.h"
#include "blackboard/Goal.h"
#include "utils/Factory.h"

#include "SelfLib.h"


class SELF_API SkillTeachingAgent : public IAgent
{
public:
	RTTI_DECL();

	SkillTeachingAgent();

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
	void OnLearningIntent(const ThingEvent & a_ThingEvent);
	void OnLearnSkill( LearningIntent::SP a_spIntent );
	void OnForgetSkill( LearningIntent::SP a_spIntent );
	void OnRetrainNLC(LearningIntent::SP a_spIntent, 
		const std::string & a_Class );
	void OnConfirmRetrain(Confirm::SP a_spConfirm);

	//! Data
	Goal::SP					m_spLastFailedGoal;
	std::list< UsedSkill::SP >	m_UsedSkills;

	std::vector<std::string>	m_LearningResponses;
	std::vector<std::string>	m_ForgetResponses;
	std::vector<std::string>	m_ConfirmRetrain;
	std::vector<std::string>	m_RetrainResponses;
	std::list<Text::SP>			m_TextHistory;
	bool						m_bEnableSkillTraining;
	bool						m_bEnableNLCTraining;
};

#endif //SELF_SKILLTEACHINGAGENT_H
