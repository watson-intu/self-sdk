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

#ifndef WDC_CREDENTIAL_H
#define WDC_CREDENTIAL_H

#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

#include "ISerializable.h"
#include "WDCLib.h"

//! Data class for holding credentials for a given service. This object
//! is contained by the Config class usually.
struct WDC_API ServiceConfig : public ISerializable, public boost::enable_shared_from_this<ServiceConfig>
{
	RTTI_DECL();

	typedef boost::shared_ptr<ServiceConfig>		SP;
	typedef boost::weak_ptr<ServiceConfig>			WP;
	typedef std::map<std::string, std::string>		CustomMap;

	ServiceConfig() 
	{}

	std::string							m_ServiceId;
	std::string							m_URL;
	std::string							m_User;
	std::string							m_Password;
	CustomMap							m_CustomMap;

	bool IsConfigured() const
	{
		return m_ServiceId.size() > 0 && m_URL.size() > 0 && (m_User.size() > 0 || m_Password.size() > 0);
	}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_ServiceId"] = m_ServiceId;
		json["m_URL"] = m_URL;
		json["m_User"] = m_User;
		json["m_Password"] = m_Password;

		SerializeMap("m_CustomMap", m_CustomMap, json);
	}

	virtual void Deserialize(const Json::Value & json)
	{
		m_ServiceId = json["m_ServiceId"].asString();
		m_URL = json["m_URL"].asString();
		m_User = json["m_User"].asString();
		m_Password = json["m_Password"].asString();

		DeserializeMap("m_CustomMap", json, m_CustomMap);
	}

	const std::string & GetKeyValue( const std::string & a_Key, const std::string & a_Default ) const
	{
		CustomMap::const_iterator iMap = m_CustomMap.find( a_Key );
		if ( iMap != m_CustomMap.end() )
			return iMap->second;

		return a_Default;
	}

	bool operator==( const ServiceConfig & a_Compare ) const
	{
		return m_ServiceId == a_Compare.m_ServiceId
			&& m_URL == a_Compare.m_URL
			&& m_User == a_Compare.m_User
			&& m_Password == a_Compare.m_Password
			&& m_CustomMap == a_Compare.m_CustomMap;
	}
	bool operator!=( const ServiceConfig & a_Compare ) const
	{
		return !operator==( a_Compare );
	}
};

#endif


