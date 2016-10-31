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

#ifndef SELF_URLSERVICE_H
#define SELF_URLSERVICE_H

#include "services/IService.h"
#include "blackboard/URL.h"
#include "SelfLib.h"

class SELF_API URLService : public IService
{
public:
	RTTI_DECL();

	//! Construction
	URLService();

	//! Data containers
	struct URLServiceData
	{
		Json::Value             m_JsonValue;
		Url::SP                 m_spUrl;
	};

	//! Typedefs
	typedef Delegate<URLServiceData *>		        UrlCallback;
	typedef struct URLServiceData                   UrlServiceData;
	typedef Delegate<void *>						OnGetServer;

	//! IService interface
	virtual bool Start();
	virtual void GetServiceStatus(ServiceStatusCallback a_Callback);

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Upload the specified dialog
	virtual void SendURL(const Url::SP & a_spUrlAgent, UrlCallback a_Callback);

	//! Callbacks
	void OnResult(const Json::Value & json);
	bool GetServerStatus(OnGetServer a_Callback);

	//! WayBlazer
	void MakeHeartBeat();
	void OnHeartBeatResponse(const Json::Value & a_Response);

	//! Escape the given URL of any spaces found in any of the arguments, this is used to convert
	//! some text into a fully escaped URL.
	static std::string EscapeUrl( const std::string & a_URL );

protected:

	//! Data
	std::string	m_AvailabilitySuffix;
	std::string	m_FunctionalSuffix;

	struct HeartBeat : public ISerializable
	{
		RTTI_DECL();

		std::string m_Message;

		virtual void Serialize(Json::Value & json)
		{
			json["message"] = m_Message;
		}

		virtual void Deserialize(const Json::Value & json)
		{
			if (json.isMember("message"))
				m_Message = json["message"].asString();
		}
	};

	//! This class is responsible for checking whether the service is available or not
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(URLService *a_pURLService, ServiceStatusCallback a_Callback);

	private:
		URLService *m_pURLService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(HeartBeat *a_pHeartBeat);
	};
};

#endif
