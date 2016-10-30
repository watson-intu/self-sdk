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

#ifndef WDC_NLC_MODELS_H
#define WDC_NLC_MODELS_H

#include "utils/ISerializable.h"

struct Classifier : public ISerializable
{
	RTTI_DECL();

	std::string m_Name;
	std::string m_Language;
	std::string m_URL;
	std::string m_ClassifierId;
	std::string m_Created;
	std::string m_Status;
	std::string m_StatusDescription;

	virtual void Serialize(Json::Value & json)
	{
		json["name"] = m_Name;
		json["language"] = m_Language;
		json["url"] = m_URL;
		json["classifier_id"] = m_ClassifierId;
		json["created"] = m_Created;
		json["status"] = m_Status;
		json["status_description"] = m_StatusDescription;
	}

	virtual void Deserialize(const Json::Value & json)
	{
		if ( json.isMember("name") )
			m_Name = json["name"].asString();
		if ( json.isMember("language") )
			m_Language = json["language"].asString();
		if ( json.isMember("url") )
			m_URL = json["url"].asString();
		if ( json.isMember("classifier_id") )
			m_ClassifierId = json["classifier_id"].asString();
		if ( json.isMember("created") )
			m_Created = json["created"].asString();
		if ( json.isMember("status") )
			m_Status = json["status"].asString();
		if ( json.isMember("status_description") )
			m_StatusDescription = json["status_description"].asString();
	}
};

struct Classifiers : public ISerializable
{
	RTTI_DECL();

	std::vector<Classifier> m_Classifiers;

	virtual void Serialize(Json::Value & json)
	{
		SerializeVector( "classifiers", m_Classifiers, json );
	}

	virtual void Deserialize(const Json::Value & json)
	{
		DeserializeVector( "classifiers", json, m_Classifiers );
	}
};

struct Class : public ISerializable
{
	RTTI_DECL();

	double m_Confidence;
	std::string m_ClassName;

	virtual void Serialize(Json::Value & json)
	{
		json["confidence"] = m_Confidence;
		json["class_name"] = m_ClassName;
	}

	virtual void Deserialize(const Json::Value & json)
	{
		if ( json.isMember("confidence") )
			m_Confidence = json["confidence"].asDouble();
		if ( json.isMember("class_name") )
			m_ClassName = json["class_name"].asString();
	}
};

struct ClassifyResult : public ISerializable
{
	RTTI_DECL();

	std::string m_ClassifierId;
	std::string m_URL;
	std::string m_Text;
	std::string m_TopClass;
	std::vector<Class> m_Classes;

	virtual void Serialize(Json::Value & json)
	{
		json["classifier_id"] = m_ClassifierId;
		json["url"] = m_URL;
		json["text"] = m_Text;
		json["top_class"] = m_TopClass;

		SerializeVector( "classes", m_Classes, json );
	}

	virtual void Deserialize(const Json::Value & json)
	{
		if ( json.isMember("classifier_id") )
			m_ClassifierId = json["classifier_id"].asString();
		if ( json.isMember("url") )
			m_URL = json["url"].asString();
		if ( json.isMember("text") )
			m_Text = json["text"].asString();
		if ( json.isMember("top_class") )
			m_TopClass = json["top_class"].asString();

		DeserializeVector( "classes", json, m_Classes );
	}

	double GetTopConfidence() const
	{
		double fTop = 0.0;
		for(size_t i=0;i<m_Classes.size();++i)
			if ( m_Classes[i].m_Confidence > fTop )
				fTop = m_Classes[i].m_Confidence;
		return fTop;
	}
};

#endif
