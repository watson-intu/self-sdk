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

#ifndef SELF_DIALOGNLC_PROXY_H
#define SELF_DIALOGNLC_PROXY_H

#include "blackboard/QuestionIntent.h"
#include "IQuestionAnswerProxy.h"

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"
#include "utils/ISerializable.h"
#include "SelfLib.h"

class Dialog;
struct Dialogs;
struct Classifiers;
struct Classifier;

class NaturalLanguageClassifier;

class SELF_API DialogNLCProxy : public IQuestionAnswerProxy
{
public:
	RTTI_DECL();

	DialogNLCProxy() : 
		m_AppendDialogClass(false),
		m_DialogUsesIntent(true),
		m_UpdateInterval( 60 * 7 ),
		m_DialogId( "" ),
		m_DialogFile( "" ),
		m_ClassifierId( "" ),
		m_ClassifierFile( "" ),
		m_Language("en"),
		m_ConversationId( 0 ), 
		m_ClientId( 0 ),
		m_bDialogStarted( false ),
		m_nUpdateOps( 0 ),
		m_bUploading(false)
	{}

	//! IQuestionAnswer Interface
	virtual void Start();
	virtual void Stop();
	virtual void AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback );

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! DialogNLCProxy specific
	void Update();
	bool DialogStarted();
	bool MatchingDialogID( const std::string & a_DialogID );

	void AskConfirm( const std::string & a_Confirm,
		Delegate<const Json::Value &> a_Callback );

private:
	//! Private callbacks
	void OnDialogStarted(const Json::Value & json);
	void OnGetDialogs(Dialogs * a_pDialogs);
	void OnDialogUploaded(const Json::Value & json);
	void OnDeleteDialog(const Json::Value & json);
	void OnGetClassifiers(Classifiers * a_pClassifiers);
	void OnTrainClassifier(Classifier * a_pClassifier);
	void OnDeleteClassifier(const Json::Value & json);
	void OnUpdated();

	//! Data
	bool						m_AppendDialogClass;		// if true, the text will be classified by NLC then appended onto the text before sending to dialog
	bool						m_DialogUsesIntent;			// if true, then the question pipeline will be passed into the dialog instead of the actual text transcript
	double						m_UpdateInterval;			// how often to check for new dialogs/classifiers

	std::string					m_DialogId;					// the dialog ID to use
	std::string					m_DialogFile;

	std::string					m_ClassifierId;				// the NLC classifier to use
	std::string					m_ClassifierFile;
	std::string					m_Language;

	Dialog *					m_pDialog;
	NaturalLanguageClassifier * m_pNLC;

	int							m_ConversationId;
	int							m_ClientId;

	bool                        m_bDialogStarted;
	TimerPool::ITimer::SP		m_spUpdateTimer;

	int							m_nUpdateOps;
	VoidDelegateList			m_OnUpdateList;
	bool						m_bUploading;


	class DialogRequest
	{
	public:
		DialogRequest(DialogNLCProxy * a_pDialogNLCProxy, QuestionIntent::SP a_spQuestion,
			Delegate<const Json::Value &> a_Callback);

		void Execute();
		void OnClassify(const Json::Value & json);
		void OnDialog(const Json::Value & json);

	private:
		DialogNLCProxy * m_pDialogNLCProxy;
		QuestionIntent::SP m_spQuestion;
		Delegate<const Json::Value &> m_Callback;
		bool m_Retried;
	};

};

#endif //SELF_DIALOGNLC_PROXY_H