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
* Limitations under the License.
*
*/

#ifndef SELF_TEXT_CLASSIFIER_H
#define SELF_TEXT_CLASSIFIER_H

#include <list>

#include "IClassifier.h"
#include "blackboard/Text.h"
#include "blackboard/IIntent.h"
#include "classifiers/proxies/ITextClassifierProxy.h"
#include "services/Alchemy/Alchemy.h"
#include "SelfLib.h"

// forward declare
struct Classifier;
struct Classifiers;
class NaturalLanguageClassifier;

//! This classifier subscribes to all audio sensors, converts any speech to text, then classifies
//! that speech into a Concept object that is added to the Blackboard.
class SELF_API TextClassifier : public IClassifier
{
public:
	RTTI_DECL();

	//! Construction
	TextClassifier();
	~TextClassifier();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IClassifier interface
	virtual const char * GetName() const;
	virtual bool OnStart();
	virtual bool OnStop();	

	//! Accessors
	double GetMinIntentConfidence() const;

	template<typename T>
	boost::shared_ptr<T> FindProxy() const
	{
		for (size_t i = 0; i < m_ClassifierProxies.size(); ++i)
		{
			boost::shared_ptr<T> spProxy = DynamicCast<T>( m_ClassifierProxies[i] );
			if (spProxy)
				return spProxy;
		}
		return boost::shared_ptr<T>();
	}

	//! Mutators
	void SetMinIntentConfidence(double a_Val);
	void SetHoldOnState(bool a_State);

	//! Types
	struct IntentClass : public ISerializable
	{
		RTTI_DECL();

		IntentClass()
		{}
		IntentClass(const std::string & a_Intent, const std::string & a_Class) :
			m_Intent(a_Intent), m_Class(a_Class)
		{}

		std::string			m_Intent;
		std::string			m_Class;

		virtual void Serialize(Json::Value & json)
		{
			json["m_Intent"] = m_Intent;
			json["m_Class"] = m_Class;
		}
		virtual void Deserialize(const Json::Value & json)
		{
			m_Intent = json["m_Intent"].asString();
			m_Class = json["m_Class"].asString();
		}
	};
	typedef std::vector<IntentClass>		IntentClasses;

private:
	//! Callback handler
	void OnText(const ThingEvent & a_ThingEvent);
	void OnHealth(const ThingEvent & a_Event);
	void OnHangOn(const ThingEvent & a_Event);
	void OnHangOnEnd();
	void OnAttention(const ThingEvent & a_Event );

	//! Types
	typedef std::vector<ITextClassifierProxy::ClassifyResult *>	ResultList;

	class ClassifyText
	{
	public:
		ClassifyText(TextClassifier * a_pClassifier, Text::SP a_spSpeech);
		~ClassifyText();

	private:
		void OnTextClassified( ITextClassifierProxy::ClassifyResult * a_Result );
		void OnTextParsed( const Json::Value & a_Parse );
		void OnTextEntities(const Json::Value & a_Parse);
		void OnClassifyDone();

		TextClassifier *	m_pClassifier;
		Text::SP			m_spText;

		ResultList			m_Results;
		Json::Value			m_Parse;

		int					m_PendingReq;
		int					m_CompletedReq;
		bool				m_bIntentCreated;
	};
	typedef std::vector<ITextClassifierProxy::SP>		ProxyList;
	typedef std::vector<std::string>					StringList;

	//! Data
	Alchemy *		m_pAlchemy;
	double			m_MinIntentConfidence;			// minimum confidence to create an intent
	double			m_MinMissNodeConfidence;		// minimum confidence to post a dialog_miss_node
	float			m_HangOnTime;					// how long to hang on

	StringList		m_FailureResponses;				// failure responses
	StringList		m_LowConfidenceResponses;       // low confidence responses
	IntentClasses	m_IntentClasses;				// maps an intent class to the correct intent class
	ProxyList		m_ClassifierProxies;			// proxies for each classifier to hit

	double          m_LastFailureResponse;
	double          m_MinFailureResponseInterval;
	bool		  	m_bHoldOn;
	TimerPool::ITimer::SP
					m_HangOnTimer;

	const std::string & GetFailureResponse();
	const std::string & GetLowConfidenceResponse();

};

inline void TextClassifier::SetMinIntentConfidence(double a_Val)
{
	m_MinIntentConfidence = a_Val;
}

inline void TextClassifier::SetHoldOnState(bool a_State)
{
	m_bHoldOn = a_State;
}

inline double TextClassifier::GetMinIntentConfidence() const
{
	return m_MinIntentConfidence;
}


#endif // SELF_TEXT_CLASSIFIER_H
