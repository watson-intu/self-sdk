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

#ifndef WDC_DIALOG_MODELS_H
#define WDC_DIALOG_MODELS_H

#include "utils/ISerializable.h"
#include "WDCLib.h"

struct WDC_API DialogEntry : public ISerializable
{
	RTTI_DECL();

	std::string m_DialogId;
	std::string m_Name;

	virtual void Serialize(Json::Value & json)
	{
		json["dialog_id"] = m_DialogId;
		json["name"] = m_Name;
	}

	virtual void Deserialize(const Json::Value & json)
	{
		m_DialogId = json["dialog_id"].asString();
		m_Name = json["name"].asString();
	}
};

struct WDC_API Dialogs : public ISerializable
{
	RTTI_DECL();

	std::vector<DialogEntry> m_Dialogs;

	virtual void Serialize(Json::Value & json)
	{
		SerializeVector( "dialogs", m_Dialogs, json );
	}

	virtual void Deserialize(const Json::Value & json)
	{
		DeserializeVector( "dialogs", json, m_Dialogs );
	}
};

#endif
