/**
* Copyright 2015 IBM Corp. All Rights Reserved.
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
* limitations under the License.
*
*/

#ifndef WDC_NLC_H
#define WDC_NLC_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "services/IService.h"
#include "DataModels.h"
#include "WDCLib.h"		// include last always

class WDC_API NaturalLanguageClassifier : public IService
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<Classifiers *>			OnGetClassifiers;
	typedef Delegate<Classifier *>			OnGetClassifier;
	typedef Delegate<Classifier *>			OnTrainClassifier;
	typedef Delegate<const Json::Value &>	OnClassify;	
	typedef Delegate<const Json::Value &>	OnDeleteClassifier;

	//! Construction 
	NaturalLanguageClassifier();

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
    virtual void GetServiceStatus( ServiceStatusCallback a_Callback );

	//! Request a list of all available classifiers
	void GetClassifiers(OnGetClassifiers a_Callback);
	//! Request all classifiers matching a given name and collect all classifier info 
	void FindClassifiers( const std::string & a_Find, OnGetClassifiers a_Callback);
	//! Request a specific classifier.
	void GetClassifier( const std::string & a_ClassifierId,
		OnGetClassifier a_Callback );
	//! Train a new classifier.
	bool TrainClassifierFile( const std::string & a_ClassifierName,
		const std::string & a_Language,
		const std::string & a_ClassifierFile,
		OnTrainClassifier a_Callback );
	void TrainClassifier( const std::string & a_ClassifierName,
		const std::string & a_Language, 
		const std::string & a_TrainingData,
		OnTrainClassifier a_Callback );
	//! Classify text
	void Classify( const std::string & a_ClassifierId,
		const std::string & a_Text, OnClassify a_Callback );
	//! Remove a classifier
	void DeleteClassifer( const std::string & a_ClassifierId,
		OnDeleteClassifier a_Callback );

private:
    //! This class is responsible for checking whether the service is available or not
    class ServiceStatusChecker
    {
    public:
        ServiceStatusChecker(NaturalLanguageClassifier *a_pNlcService, ServiceStatusCallback a_Callback);

    private:
        NaturalLanguageClassifier *m_pNlcService;
        IService::ServiceStatusCallback m_Callback;

        void OnCheckService(Classifiers *a_pClassifiers);
    };

	class FindRequest
	{
	public:
		FindRequest(NaturalLanguageClassifier * a_pService, const std::string & a_Find, OnGetClassifiers a_Callback);

	private:
		NaturalLanguageClassifier * m_pService;
		OnGetClassifiers m_Callback;
		std::string m_Find;
		int m_nPendingRequests;
		Classifiers * m_pClassifiers;

		void OnClassifiers(Classifiers * a_pClassifiers);
		void OnClassifier(Classifier * a_pClassifier);
	};

};

#endif
