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
* limitations under the License.
*
*/

#include "FaceClassifier.h"

REG_SERIALIZABLE(FaceClassifier);
RTTI_IMPL(FaceClassifier, IClassifier);


void FaceClassifier::Serialize(Json::Value & json)
{
	IClassifier::Serialize(json);
}

void FaceClassifier::Deserialize(const Json::Value & json)
{
	IClassifier::Deserialize(json);
}

const char * FaceClassifier::GetName() const
{
	return "FaceClassifier";
}

bool FaceClassifier::OnStart()
{
	BlackBoard * pBlackboard = SelfInstance::GetInstance()->GetBlackBoard();
	pBlackboard->SubscribeToType(Person::GetStaticRTTI(),
		DELEGATE(FaceClassifier, OnPerson, const ThingEvent &, this), TE_ADDED);

	FacialRecognition * pFacialRecognition = SelfInstance::GetInstance()->FindService<FacialRecognition>();
	if (pFacialRecognition == NULL)
	{
		Log::Error("FaceClassifier", "FaceRecognition service not available.");
		return false;
	}
	Log::Status("FaceClassifier", "FaceClassifier started");
	return true;
}

bool FaceClassifier::OnStop()
{
	Log::Status("FaceClassifier", "Face Classifier stopped");
	BlackBoard * pBlackboard = SelfInstance::GetInstance()->GetBlackBoard();
	pBlackboard->UnsubscribeFromType(Person::GetStaticRTTI(), this);
	return true;
}

class ClassifyFaces
{
public:
	ClassifyFaces(Image::SP a_spImage, Person::SP a_spPerson) : m_spImage(a_spImage), m_spPerson(a_spPerson)
	{
		FacialRecognition * pFacialRecognition = SelfInstance::GetInstance()->FindService<FacialRecognition>();
		if (pFacialRecognition != NULL)
		{
			pFacialRecognition->IdentifyFaces(m_spImage->GetContent(),
				DELEGATE(ClassifyFaces, OnIdentifyFaces, Identify *, this));

		}
	}

private:
	void OnIdentifyFaces(Identify * a_pIdentify)
	{
		if (a_pIdentify != NULL)
		{
			m_spPerson->SetName(a_pIdentify->m_Name);
			m_spPerson->SetConfidence(a_pIdentify->m_Confidence);
			Log::Debug("FaceClassifier", "Found Person %s (%f)", m_spPerson->GetName().c_str(), m_spPerson->GetConfidence());
			m_spPerson->SetState("named");
		}
		delete this;
	}

	Image::SP		m_spImage;
	Person::SP      m_spPerson;
};


void FaceClassifier::OnPerson(const ThingEvent & a_ThingEvent)
{
	Person::SP spPerson = DynamicCast<Person>(a_ThingEvent.GetIThing());
	if (spPerson)
	{
		Log::Debug("FaceClassifier", "Classifying Face");
		Image::SP spImage = DynamicCast<Image>(spPerson->GetParent());
		if (spImage)
			new	ClassifyFaces(spImage, spPerson);
	}
}


