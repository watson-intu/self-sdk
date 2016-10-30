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

#ifndef ROBOT_GATEWAY_MODELS_H
#define ROBOT_GATEWAY_MODELS_H

#include "utils/ISerializable.h"
#include "SelfLib.h"

struct ServiceAttributes : public ISerializable
{
	RTTI_DECL();

	std::string		m_Key;
	std::string		m_Value;

	virtual void Serialize(Json::Value & json)
	{
		json[m_Key] = m_Value;
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		for (Json::Value::iterator it = json.begin(); it != json.end(); ++it)
		{
			m_Key = it.key().asString();
			m_Value = (*it).asString();
		}
	}
};

struct Service : public ISerializable
{
	RTTI_DECL();

	std::string             m_ServiceName;
	std::string		        m_Endpoint;
	std::string		        m_Username;
	std::string		        m_Password;
	std::string		        m_ServiceStatus;
	std::vector <ServiceAttributes>
							m_ServiceAttributes;

	//!ISerizliable interface
	virtual void Serialize(Json::Value & json)
	{
		json["serviceName"] = m_ServiceName;
		json["endpoint"] = m_Endpoint;
		json["username"] = m_Username;
		json["password"] = m_Password;
		json["serviceStatus"] = m_ServiceStatus;

		SerializeVector( "serviceAttributes", m_ServiceAttributes, json );
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_ServiceName = json["serviceName"].asString();
		m_Endpoint = json["endpoint"].asString();
		m_Username = json["username"].asString();
		m_Password = json["password"].asString();
		m_ServiceStatus = json["serviceStatus"].asString();

		DeserializeVector( "serviceAttributes", json, m_ServiceAttributes );
	}
};

struct ServiceList : public ISerializable
{
	RTTI_DECL();

	std::string             m_OrgId;
	std::string             m_GroupId;
	std::vector<Service>    m_Services;

	virtual void Serialize(Json::Value & json)
	{
		json["orgId"] = m_OrgId;
		json["groupId"] = m_GroupId;
		SerializeVector( "services", m_Services, json );
	}

	virtual void Deserialize(const Json::Value & json) 
	{
		m_OrgId = json["orgId"].asString();
		m_GroupId = json["groupId"].asString();
		DeserializeVector( "services", json, m_Services );
	}
};

#endif //ROBOT_GATEWAY_MODELS_H
