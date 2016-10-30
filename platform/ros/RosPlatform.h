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

#ifndef SELF_ROSPLATFORM_H
#define SELF_ROSPLATFORM_H

#include <boost/thread.hpp>

#include "utils/TimerPool.h"
#include "utils/IWebClient.h"
#include "SelfInstance.h"


class RosPlatform
{
public:
    static RosPlatform * Instance();

    //! Construction
    RosPlatform( const std::string & a_URL = "" );
    ~RosPlatform();

    void    ConnectToRobot();
    void    OnClientState( IWebClient * a_pClient );
    bool    Publish(const std::string & a_TopicId, const std::string & a_Message, const Json::Value & a_Data);
    void    Advertise( const std::string & a_TopicId, const std::string & a_Message);
    bool	Subscribe(const std::string & a_Path, const std::string & a_Message, Delegate<const Json::Value &> a_Callback);
    bool    Unsubscribe( const std::string & a_Path, void * a_pObject );

    bool IsConnected()
    {
        return m_bActive;
    }

private:

    //! Types
    typedef Delegate<const Json::Value &>			MessageHandler;
	struct Subscription
	{
		MessageHandler	m_Handler;
		std::string		m_TopicId;
		std::string		m_SubId;
	};
	typedef std::multimap<std::string, Subscription>		SubscriptionMap;
	
    TimerPool::ITimer::SP                       m_spReconnectTimer;
    IWebClient::SP	                            m_spWebClient;
    std::vector<std::string>                    m_IdMap;
    std::vector<std::string>                    m_Advertisements;
    float			                            m_fReconnectInterval;
    std::string                                 m_URL;
    bool			                            m_bActive;
    int                                         m_Counter;
	SubscriptionMap								m_SubscriptionMap;
	boost::mutex								m_SubscriptionLock;

    void HandleSubscribe(const Json::Value & a_Message);
    void HandlePublish(const Json::Value & a_Message);
	void OnFrame(IWebSocket::FrameSP a_spFrame);

    static RosPlatform * sm_pInstance;

};
#endif //SELF_ROSPLATFORM_H
