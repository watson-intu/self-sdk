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

#ifndef SELF_VISUALTEACHINGAGENT_H
#define SELF_VISUALTEACHINGAGENT_H

#include "IAgent.h"
#include "blackboard/LearningIntent.h"
#include "blackboard/Image.h"
#include "blackboard/Entity.h"
#include "utils/Factory.h"

#include "SelfLib.h"


class SELF_API VisualTeachingAgent : public IAgent
{
public:
	RTTI_DECL();

	VisualTeachingAgent();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Callbacks
	void OnImage(const ThingEvent & a_ThingEvent);
	void OnLearningIntent(const ThingEvent & a_ThingEvent);
	void OnLearnObject( LearningIntent::SP a_spIntent );
	void OnForgetObject( LearningIntent::SP a_spIntent );

	//! Data
	std::vector<std::string>	m_LearnObjectResponses;
	std::vector<std::string>	m_ForgetObjectResponses;
	std::vector<std::string>    m_FailedImageTraining;
	std::list<Image::SP>		m_ImageHistory;
	bool						m_bEnableObjectTraining;
};

#endif //SELF_VISUALTEACHINGAGENT_H
