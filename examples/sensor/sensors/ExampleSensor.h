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

#ifndef SELF_EXAMPLESENSOR_H
#define SELF_EXAMPLESENSOR_H

#include "utils/IWebClient.h"
#include "utils/ThreadPool.h"
#include "utils/ParamsMap.h"
#include "utils/TimerPool.h"
#include "sensors/RemoteDeviceData.h"
#include "sensors/ISensor.h"
#include "SelfLib.h"

//! This is the base class for a interacting with external devices
//! For example, weather station, cameras, microphones
class ExampleSensor : public ISensor
{
public:
	RTTI_DECL();

	struct Rest : public ISerializable
	{
		RTTI_DECL();

		typedef IWebClient::Headers		Headers;

		Rest()
		{}

		//Declare the strings to take values from the body.JSON 
		std::string         m_Token;
		std::string         m_URL;
		std::string         m_Type;
		std::string			m_Params;
		//std::string         m_Body;
		Headers             m_Headers;

		//Serialize the values from the boday.JSON 
		virtual void Serialize(Json::Value & json)
		{
			json["m_URL"] = m_URL;
			json["m_Type"] = m_Type;
			//json["m_Body"] = m_Body;
			json["m_Params"] = m_Params;

			int index = 0;
			for( Headers::iterator iHeader = m_Headers.begin(); iHeader != m_Headers.end(); ++iHeader )
			{
				json["m_Headers"][index]["key"] = iHeader->first;
				json["m_Headers"][index++]["value"] = iHeader->second;
			}
		}

		//Deserialize the values from the body.JSON
		virtual void Deserialize(const Json::Value & json)
		{
			// Required Fields
			m_URL = json["m_URL"].asString();
			m_Type = json["m_Type"].asString();

			// Optional Fields
			//if( json.isMember( "m_Body" ) )
				//m_Body = json["m_Body"].asString();
			if( json.isMember( "m_Params" ) )
				m_Params = json["m_Params"].asString();
			m_Headers.clear();
			if ( json.isMember( "m_Headers" ) )
			{
				const Json::Value & headers = json["m_Headers"];
				for( Json::ValueConstIterator iHeader = headers.begin(); iHeader != headers.end(); ++iHeader )
					m_Headers[ (*iHeader)["key"].asString() ] = (*iHeader)["value"].asString();
			}
		}
	};

	//Definging the sensor time
	//! Construction
	ExampleSensor() : m_fPollInterval( 1.0f ), m_Paused( 0 )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "RemoteDevice";
	}
	virtual const char * GetDataType()
	{
		return RemoteDeviceData::GetStaticRTTI().GetName().c_str();
	}

	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

	void SendingData( RemoteDeviceData * a_pData );

private:
	//! Types
	class DeviceRequest
	{
	public:
		DeviceRequest();
		DeviceRequest(const std::vector<ExampleSensor::Rest> & Rests, ExampleSensor * a_pDevice);
		~DeviceRequest();

		void OnState(IWebClient * a_pConnector);
		void OnResponse(IWebClient::RequestData * a_pResponse);
		void MakeRequest();

	private:
		size_t m_Index;
		ExampleSensor * m_pDevice;
		std::vector<ExampleSensor::Rest> m_Rests;
		IWebClient::SP m_spClient;
		ParamsMap m_Param;
	};

	//!Data
	float					m_fPollInterval;
	std::vector<Rest>       m_Rests;
	TimerPool::ITimer::SP   m_spWaitTimer;
	volatile int			m_Paused;

	void                    StreamingThread();
};

#endif
