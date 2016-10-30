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

#ifndef WDC_RELATIONSHIP_EXTRACTION_H
#define WDC_RELATIONSHIP_EXTRACTION_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "services/IService.h"
#include "WDCLib.h"		// include last always

class WDC_API RelationshipExtraction : public IService
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<const Json::Value &>	OnParse;

	//! Construction 
	RelationshipExtraction();

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
	virtual void GetServiceStatus(ServiceStatusCallback a_Callback);

	//! Accessors
	const std::string & GetLanguage() const
	{
		return m_Language;
	}

	void SetLanguage(const std::string & a_Language)
	{
		m_Language = a_Language;
	}

	//! Parse the provided text and invoked the callback with the results.
	bool Parse(const std::string & a_Text, OnParse a_Callback);

	//! parse a syntactic parse into a json tree structure.
	static bool ParseIntoTree(const std::string & a_Parse, Json::Value & a_Tree);
	//! This helper function is used by ParseIntoTree to break up a string in it's parts based on brackets [] 
	static bool SplitParseParts(std::string & a_Parse, std::vector<std::string> & a_Parts);
private:
	//! Types
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(RelationshipExtraction * a_pService, 
			ServiceStatusCallback a_Callback);

	private:
		RelationshipExtraction *		m_pService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(const Json::Value &);
	};

	//! Data
	std::string			m_Language;
};

#endif
