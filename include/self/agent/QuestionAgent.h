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

#ifndef SELF_QUESTIONAGENT_H
#define SELF_QUESTIONAGENT_H

#include "IAgent.h"
#include "utils/Factory.h"
#include "blackboard/QuestionIntent.h"
#include "blackboard/Confirm.h"
#include "proxies/IQuestionAnswerProxy.h"
#include "SelfLib.h"

class SELF_API QuestionAgent : public IAgent
{
public:
	RTTI_DECL();

	//! Construction
	QuestionAgent();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);
	
	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

	void OnHealth( const ThingEvent & a_Event );

private:
	//! Types
	class ProcessQuestion
	{
	public:
		ProcessQuestion(QuestionAgent * a_pAgent, QuestionIntent::SP a_spQuestion);
		~ProcessQuestion();

	private:
		//! Callbacks
		void OnAnswer(const Json::Value & json);
		void OnFinalResult();
		void OnConfirm(Confirm::SP a_spConfirm);

		QuestionAgent *		m_pAgent;
		QuestionIntent::SP	m_spQuestion;
		int					m_PendingRequests;
		int					m_CompletedRequests;
		bool				m_bGoalCreated;
		std::vector<Json::Value>	
							m_Responses;
	};

	

	//! Data
	double						m_MinDialogConfidence;
	double						m_MinAnswerConfidence;
	double						m_UseDialogConfidence;		
	std::string					m_DialogMissIntent;
	float						m_HangOnTime;
	int							m_nQuestionLimit;		// maximum number of active questions 
	std::string					m_ConfirmationTag;			// tag in a dialog response for a confirmation

	std::vector<IQuestionAnswerProxy::SP>	
								m_QuestionAnswerProxies;				// one or more dialogs to query
	std::vector<std::string>	m_HangOnResponses;
	std::vector<std::string>	m_PipelineDownResponses;

	TimerPool::ITimer::SP		m_spHangOnTimer;
	bool						m_IgnoreMissedDialogNodes;
	std::list<ProcessQuestion *>
								m_ActiveQuestions;

    void OnQuestionIntent(const ThingEvent & a_ThingEvent);
	void OnHangOnIntent(const ThingEvent & a_ThingEvent);
	void StopIgnoring();
};

#endif //SELF_QUESTIONAGENT_H
