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

#include "ExampleSensor.h"

REG_SERIALIZABLE( ExampleSensor );
RTTI_IMPL(ExampleSensor, ISensor);
RTTI_IMPL(ExampleSensor::Rest, ISerializable );

void ExampleSensor::Serialize(Json::Value & json)
{
	Log::Debug("ExampleSensor", "Serializing Object");
	json["m_fPollInterval"] = m_fPollInterval;
	SerializeVector("m_Rests", m_Rests, json);
}

void ExampleSensor::Deserialize(const Json::Value & json)
{
	if (json.isMember("m_fPollInterval"))
		m_fPollInterval = json["m_fPollInterval"].asFloat();
	DeserializeVector("m_Rests", json, m_Rests);
}

bool ExampleSensor::OnStart()
{
	Log::Debug("ExampleSensor", "Starting up Remote Device");
	m_spWaitTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(ExampleSensor, StreamingThread, this), m_fPollInterval, true, true);
	return true;
}

void ExampleSensor::StreamingThread()
{
	if (m_Paused <= 0 )
		new DeviceRequest(m_Rests, this);
}

bool ExampleSensor::OnStop()
{
	m_spWaitTimer.reset();
	return true;
}

void ExampleSensor::SendingData( RemoteDeviceData * a_pData )
{
	SendData( a_pData );
}

void ExampleSensor::OnPause()
{
	m_Paused++;
}

void ExampleSensor::OnResume()
{
	m_Paused--;
}

//--------------------------------------

ExampleSensor::DeviceRequest::DeviceRequest()
{}

ExampleSensor::DeviceRequest::DeviceRequest(const std::vector<ExampleSensor::Rest> & Rests, ExampleSensor * a_pDevice) :
	m_pDevice(a_pDevice), m_Rests(Rests), m_Index(0)
{
	MakeRequest();
}

ExampleSensor::DeviceRequest::~DeviceRequest()
{}

void ExampleSensor::DeviceRequest::OnState(IWebClient * a_pConnector)
{
	if (a_pConnector->GetState() == IWebClient::CLOSED || a_pConnector->GetState() == IWebClient::DISCONNECTED)
	{
		if (m_Index < (m_Rests.size() - 1) )
		{
			m_Index += 1;

			Json::Value req;
			m_Rests[m_Index].Serialize(req);
			req = m_Param.ResolveVariables(req);
			m_Rests[m_Index].Deserialize(req);
			MakeRequest();
		}
		else
		{
			Log::Debug("ExampleSensor", "Received data: %s", m_Param["response"][m_Index].toStyledString().c_str() );
			ThreadPool::Instance()->InvokeOnMain<RemoteDeviceData *>(
				DELEGATE(ExampleSensor, SendingData, RemoteDeviceData *, m_pDevice),
				new RemoteDeviceData(m_Param["response"][m_Index]));

			delete this;
		}
	}
}

void ExampleSensor::DeviceRequest::OnResponse(IWebClient::RequestData * a_pResponse)
{
	Json::Value root;
	Json::Reader reader;
	reader.parse(a_pResponse->m_Content, root);
	m_Param["response"][m_Index] = root;
}

void ExampleSensor::DeviceRequest::MakeRequest()
{
	if (!m_spClient)
		m_spClient = IWebClient::Create();
	
	m_Rests[m_Index].m_Headers["Authorization"] = "Bearer 1ecee119-299e-41fa-9011-3593a73a1c33";
	m_spClient->Request(m_Rests[m_Index].m_URL + m_Rests[m_Index].m_Params,
		m_Rests[m_Index].m_Headers,
		m_Rests[m_Index].m_Type,
		"",
		DELEGATE(DeviceRequest, OnResponse, IWebClient::RequestData *, this),
		DELEGATE(DeviceRequest, OnState, IWebClient *, this));
}
