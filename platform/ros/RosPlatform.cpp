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

#include "RosPlatform.h"
#include "utils/StringUtil.h"

RosPlatform * RosPlatform::sm_pInstance = NULL;

RosPlatform * RosPlatform::Instance()
{
    static tthread::mutex lock;
    tthread::lock_guard<tthread::mutex> guard( lock );
    if ( sm_pInstance == NULL )
        new RosPlatform();

    return sm_pInstance;
}

RosPlatform::RosPlatform( const std::string & a_URL /*= ""*/ )
    : m_URL(a_URL), m_fReconnectInterval(5.0f), m_bActive(false), m_Counter(0)
{
    Log::Debug("RosPlatform", "Instantiating Ros Platform");
    sm_pInstance = this;

    SelfInstance * pInstance = SelfInstance::GetInstance();
    if ( pInstance == NULL )
        m_URL = "ws://192.168.1.89:9090";
    else
        m_URL = pInstance->GetRobotUrl();

    StringUtil::Replace(m_URL, "https://", "ws://", true );
    StringUtil::Replace(m_URL, "http://", "ws://", true );
    // start a timer to check for a connection every 5 seconds..
    SelfInstance * pSelf = SelfInstance::GetInstance();
    if ( pSelf != NULL )
    {
        m_spReconnectTimer = pSelf->GetTimerPool()->StartTimer(
                VOID_DELEGATE( RosPlatform, ConnectToRobot, this ), m_fReconnectInterval, true, true );
    }
    // go ahead and try to connect now..
    ConnectToRobot();
}

RosPlatform::~RosPlatform()
{
    m_bActive = false;

    if (m_spWebClient)
        m_spWebClient->Shutdown();

    m_spReconnectTimer.reset();
}

void RosPlatform::ConnectToRobot()
{
    Log::Debug("RosPlatform", "Connected to Robot");
    m_spReconnectTimer.reset();

    if (!m_spWebClient)
        m_spWebClient = IWebClient::SP(IWebClient::Create());
    m_spWebClient->SetURL(m_URL);
    m_spWebClient->SetStateReceiver(DELEGATE(RosPlatform, OnClientState, IWebClient *, this));
	m_spWebClient->SetFrameReceiver(DELEGATE(RosPlatform, OnFrame, IWebSocket::FrameSP, this));

    Log::Status("RosPlatform", "Connecting to robot %s", m_URL.c_str());
    if (!m_spWebClient->Send())
    {
        Log::Error("RosPlatform", "Send() return false.");
        m_bActive = false;
    }
    m_bActive = true;
}

void RosPlatform::OnClientState( IWebClient * a_pClient )
{
    Log::Debug("RosPlatform", "OnClientState() %u", a_pClient->GetState());
    switch (a_pClient->GetState())
    {
        case IWebClient::CONNECTING:		// trying to establish a connection
        case IWebClient::CONNECTED:			// connection established, ready to send/receive data
        case IWebClient::CLOSING:			// set when Close() is invoked and before it's really closed
            break;
        case IWebClient::CLOSED:			// connection has been closed gracefully
        case IWebClient::DISCONNECTED:		// connection has been lost
        {
            Log::Debug("RosPlatform", "Connection to robot closed/disconnected.");
            if (m_bActive)
            {
                if (TimerPool::Instance() != NULL)
                {
                    Log::Status("RosPlatform", "Disconnected -- Will try to connect to robot in %f seconds.", m_fReconnectInterval);
                    m_spReconnectTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(RosPlatform, ConnectToRobot, this), m_fReconnectInterval, true, false);
                }
                else
                    Log::Warning("RosPlatform", "No TimerPool, unable to start automatic reconnect.");
                m_bActive = false;

            }
            break;
        }
    }
}

bool RosPlatform::Publish(
        const std::string & a_TopicId,
        const std::string & a_Message,
        const Json::Value & a_Data)
{
    Advertise(a_TopicId, a_Message);
    std::string publishId = StringUtil::Format("publish: %s: %d", a_TopicId.c_str(), ++m_Counter);
    m_IdMap.push_back(publishId);
    Json::Value json;
    json["op"] = "publish";
    json["id"] = publishId;
    json["topic"] = a_TopicId;
    json["msg"] = a_Data;
    Log::Debug("RosPlatform", "Sending Publish: %s", Json::FastWriter().write( json ).c_str());
    m_spWebClient->SendText( Json::FastWriter().write( json ) );

    return true;
}

void RosPlatform::Advertise( const std::string & a_TopicId, const std::string & a_Message)
{
    if (std::find(m_Advertisements.begin(), m_Advertisements.end(), a_TopicId) == m_Advertisements.end())
    {
        std::string publishId = StringUtil::Format("advertise: %s: %d", a_TopicId.c_str(), ++m_Counter);
        m_IdMap.push_back(publishId);
        m_Advertisements.push_back(a_TopicId);
        Json::Value json;
        json["op"] = "advertise";
        json["id"] = publishId;
        json["type"] = a_Message;
        json["topic"] = a_TopicId;
        Log::Debug("RosPlatform", "Sending Advertisement: %s", Json::FastWriter().write( json ).c_str());
        m_spWebClient->SendText( Json::FastWriter().write( json ) );
    }
}

bool RosPlatform::Subscribe(
        const std::string & a_Path,
        const std::string & a_Message,
        Delegate<const Json::Value &> a_Callback)
{
	boost::lock_guard<boost::mutex> lock(m_SubscriptionLock);
	Subscription sub;
	sub.m_Handler = a_Callback;
	sub.m_TopicId = a_Path;
	sub.m_SubId = UniqueID().Get();
  
	Json::Value json;
    json["op"] = "subscribe";
    json["id"] = sub.m_SubId;
    json["type"] = a_Message;
    json["topic"] = a_Path;
    json["compression"] = "none";
    json["throttle_rate"] = 0;

    m_spWebClient->SendText( Json::FastWriter().write( json ) );
	m_SubscriptionMap.insert(SubscriptionMap::value_type(a_Path, sub));

	return true;
}

bool RosPlatform::Unsubscribe( const std::string & a_Path, void * a_pObject  )
{
	boost::lock_guard<boost::mutex> lock(m_SubscriptionLock);
	SubscriptionMap::iterator iSub = m_SubscriptionMap.find(a_Path);
	while (iSub == m_SubscriptionMap.end())
	{
		if (iSub->first != a_Path)
			break;
		if (iSub->second.m_Handler.IsObject(a_pObject))
		{
			Json::Value json;
			json["op"] = "unsubscribe";
			json["id"] = iSub->second.m_SubId;
			json["topic"] = iSub->second.m_TopicId;

			m_spWebClient->SendText(Json::FastWriter().write(json));
			m_SubscriptionMap.erase(iSub);
			return true;
		}

		++iSub;
	}

    return false;
}

void RosPlatform::OnFrame(IWebSocket::FrameSP a_spFrame)
{
	if (a_spFrame->m_Op == IWebSocket::TEXT_FRAME)
	{
		Log::Debug("RosPlatform", "Received text frame: %s", a_spFrame->m_Data.c_str());

		Json::Value root;
		Json::Reader reader( Json::Features::strictMode() );
		if (reader.parse(a_spFrame->m_Data, root))
		{
			if (root.isMember("topic"))
			{
				// {"topic": "/goal_status", "msg" : {"header": {"stamp": {"secs": 1467044592, "nsecs" : 175644067}, "frame_id" : "TrajectoryManager", "seq" : 298}, "status_list" : [{"status": 0, "text" : "Joint trajectory received at follower.", "goal_id" : {"stamp": {"secs": 0, "nsecs" : 0}, "id" : "b2ba1b51-6ae9-4550-ad3b-86f01979a17b"}}]}, "op" : "publish"}
				const std::string & topic = root["topic"].asString();

				boost::lock_guard<boost::mutex> lock(m_SubscriptionLock);
				SubscriptionMap::iterator iSub = m_SubscriptionMap.find(topic);
				if (iSub != m_SubscriptionMap.end())
				{
					while (iSub != m_SubscriptionMap.end())
					{
						if (iSub->first != topic)
							break;

						// invoke the handler
						(*iSub).second.m_Handler(root);
						++iSub;
					}
				}
				else
				{
					Log::Warning("RosPlatform", "Unhandled topic %s", topic.c_str());
				}
			}
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

