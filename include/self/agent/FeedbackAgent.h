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

#ifndef SELF_FEEDBACKAGENT_H
#define SELF_FEEDBACKAGENT_H

#include "IAgent.h"
#include "blackboard/Confirm.h"
#include "blackboard/IIntent.h"
#include "utils/Factory.h"
#include "utils/TimerPool.h"

#include "SelfLib.h"

class SELF_API FeedbackAgent : public IAgent
{
public:
    RTTI_DECL();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Callbacks
	void OnLearningIntent(const ThingEvent & a_ThingEvent);
	void OnIntents(const ThingEvent & a_ThingEvent);
	void OnConfirm(const ThingEvent & a_ThingEvent);

	//! Data
	std::list<Confirm::SP>		m_Confirmations;
	IIntent::SP					m_spLastIntent;

	std::vector<std::string>	m_PositiveResponses;
	std::vector<std::string>	m_NegativeResponses;
	std::vector<std::string>	m_QuestionPositiveResponses;
	std::vector<std::string>	m_QuestionNegativeResponses;
};


#endif //SELF_FEEDBACKAGENT_H
