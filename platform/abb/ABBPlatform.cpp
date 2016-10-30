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

#include "ABBPlatform.h"
#include "utils/StringUtil.h"

ABBPlatform * ABBPlatform::sm_pInstance = NULL;

ABBPlatform * ABBPlatform::Instance()
{
	static tthread::mutex lock;
	tthread::lock_guard<tthread::mutex> guard(lock);
	if (sm_pInstance == NULL)
		new ABBPlatform();

	return sm_pInstance;
}

ABBPlatform::ABBPlatform(const std::string & a_URL /*= ""*/)
	: m_URL(a_URL), m_fReconnectInterval(5.0f), m_bActive(false)
{
	Log::Debug("ABBPlatform", "Instantiating ABB Platform");
	sm_pInstance = this;

	SelfInstance * pInstance = SelfInstance::GetInstance();
	if (pInstance == NULL)
		m_URL = "ws://127.0.0.1:9090";
	else
		m_URL = pInstance->GetRobotUrl();

	StringUtil::Replace(m_URL, "https://", "ws://", true);
	StringUtil::Replace(m_URL, "http://", "ws://", true);
	// start a timer to check for a connection every 5 seconds..
	SelfInstance * pSelf = SelfInstance::GetInstance();
	if (pSelf != NULL)
	{
		m_spReconnectTimer = pSelf->GetTimerPool()->StartTimer(
			VOID_DELEGATE(ABBPlatform, ConnectToRobot, this), m_fReconnectInterval, true, true);
	}
	// go ahead and try to connect now..
	ConnectToRobot();
}

ABBPlatform::~ABBPlatform()
{
	m_bActive = false;

	if (m_spWebClient)
		m_spWebClient->Shutdown();

	m_spReconnectTimer.reset();
}

void ABBPlatform::ConnectToRobot()
{
	Log::Debug("ABBPlatform", "Connecting to Robot");
	m_spReconnectTimer.reset();

	if (!m_spWebClient)
		m_spWebClient = IWebClient::SP(IWebClient::Create());
	m_spWebClient->SetURL(m_URL);
	m_spWebClient->SetStateReceiver(DELEGATE(ABBPlatform, OnClientState, IWebClient *, this));
	m_spWebClient->SetFrameReceiver(DELEGATE(ABBPlatform, OnFrame, IWebSocket::FrameSP, this));

	Log::Status("ABBPlatform", "Connecting to robot %s", m_URL.c_str());
	if (!m_spWebClient->Send())
	{
		Log::Error("ABBPlatform", "Send() return false.");
		m_bActive = false;
	}
	m_bActive = true;
}

void ABBPlatform::OnClientState(IWebClient * a_pClient)
{
	Log::Debug("ABBPlatform", "OnClientState() %u", a_pClient->GetState());
	switch (a_pClient->GetState())
	{
	case IWebClient::CONNECTING:		// trying to establish a connection
	case IWebClient::CONNECTED:			// connection established, ready to send/receive data
	case IWebClient::CLOSING:			// set when Close() is invoked and before it's really closed
		break;
	case IWebClient::CLOSED:			// connection has been closed gracefully
	case IWebClient::DISCONNECTED:		// connection has been lost
	{
		Log::Debug("ABBPlatform", "Connection to robot closed/disconnected.");
		if (m_bActive)
		{
			if (TimerPool::Instance() != NULL)
			{
				Log::Status("ABBPlatform", "Disconnected -- Will try to connect to robot in %f seconds.", m_fReconnectInterval);
				m_spReconnectTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(ABBPlatform, ConnectToRobot, this), m_fReconnectInterval, true, false);
			}
			else
				Log::Warning("ABBPlatform", "No TimerPool, unable to start automatic reconnect.");
			m_bActive = false;

		}
		break;
	}
	}
}

bool ABBPlatform::Send(
	const std::string & a_TopicId,
	const float a_Message)
{
	Json::Value json;
	json["Arm"] = a_TopicId;
	json["Module"] = a_Message;
	Log::Debug("ABBPlatform", "Sending Publish: %s", Json::FastWriter().write(json).c_str());
	m_spWebClient->SendText(Json::FastWriter().write(json));

	return true;
}

void ABBPlatform::OnFrame(IWebSocket::FrameSP a_spFrame)
{
	if (a_spFrame->m_Op == IWebSocket::TEXT_FRAME)
	{
		Log::Debug("ABBPlatform", "Received text frame: %s", a_spFrame->m_Data.c_str());

		Json::Value root;
		Json::Reader reader(Json::Features::strictMode());
		if (reader.parse(a_spFrame->m_Data, root))
		{
			Log::Debug("ABBPlatform", "Received the following response from ABB: %s", Json::FastWriter().write(root).c_str());
		}
		else
		{
			Log::Error("RosPLatform", "Failed to parse json: %s", a_spFrame->m_Data.c_str());
		}
	}
	else
	{
		Log::Debug("RosPlatform", "Unhanded web frame %d", (int)a_spFrame->m_Op);
	}
}

