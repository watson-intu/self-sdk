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

#ifndef SELF_ABBPLATFORM_H
#define SELF_ABBPLATFORM_H

#include <boost/thread.hpp>

#include "utils/TimerPool.h"
#include "utils/IWebClient.h"
#include "SelfInstance.h"


class ABBPlatform
{
public:
	static ABBPlatform * Instance();

	//! Construction
	ABBPlatform(const std::string & a_URL = "");
	~ABBPlatform();

	void    ConnectToRobot();
	void    OnClientState(IWebClient * a_pClient);
	bool    Send(const std::string & a_TopicId, float a_Message);

	bool IsConnected()
	{
		return m_bActive;
	}

private:

	TimerPool::ITimer::SP                       m_spReconnectTimer;
	IWebClient::SP	                            m_spWebClient;
	float			                            m_fReconnectInterval;
	std::string                                 m_URL;
	bool			                            m_bActive;

	void OnFrame(IWebSocket::FrameSP a_spFrame);

	static ABBPlatform * sm_pInstance;

};
#endif //SELF_ABBPLATFORM_H
