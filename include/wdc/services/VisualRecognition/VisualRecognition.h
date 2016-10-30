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

#ifndef WDC_VISUALRECOGNITION_H
#define WDC_VISUALRECOGNITION_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "services/IService.h"
#include "WDCLib.h"		// include last always

class WDC_API VisualRecognition : public IService
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<const Json::Value &>	OnClassifyImage;
	typedef Delegate<const Json::Value &>	OnDetectFaces;
	typedef Delegate<const Json::Value &>   OnIdentifyText;
	typedef Delegate<const Json::Value &>   OnClassifierTrained;

	//! Construction 
	VisualRecognition();

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
	virtual void GetServiceStatus(ServiceStatusCallback a_Callback);

	//! Classify the given image and returns image tags for the image data.
	void ClassifyImage(const std::string & a_ImageData,
		const std::string & classifierId,
		OnClassifyImage a_Callback,
		bool a_bKnowledgeGraph = false );
	//! Detect faces in the provided image 
	void DetectFaces(const std::string & a_ImageData,
		OnDetectFaces a_Callback, 
		bool a_bKnowledgeGraph = false );
	//! Identify the Text within a provided image
	void IdentifyText(const std::string & a_ImageData,
		OnIdentifyText a_Callback,
		bool a_bKnowledgeGraph = false );
	//! Retrains the Image Classifier with positive examples
	void TrainClassifierPositives(const std::string & a_ImageData,
		std::string & classifierId,
		std::string & classifierName,
		const std::string & imageClass,
		OnClassifierTrained a_Callback);
	//! Retrains the Image Classifier with negative examples
	void TrainClassifierNegatives(const std::string & a_ImageData,
		std::string & classifierId,
		std::string & classifierName,
		const std::string & imageClass,
		OnClassifierTrained a_Callback);

private:
	//! Types
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(VisualRecognition * a_pService,
			ServiceStatusCallback a_Callback);

	private:
		VisualRecognition *				m_pService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(const Json::Value &);
	};

};

#endif
