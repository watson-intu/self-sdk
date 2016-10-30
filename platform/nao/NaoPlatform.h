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

#ifndef NAO_PLATFORM_H
#define NAO_PLATFORM_H

#include "utils/TimerPool.h"
#include "SelfInstance.h"

#pragma warning(disable:4275)
#include "qi/session.hpp"

//! Manager class for managing the connection to a remote/local robot.
class NaoPlatform
{
public:
	static NaoPlatform * Instance();

	//! Construction
	NaoPlatform( const std::string & a_URL = "" );
	~NaoPlatform();

	//! Accessors
	qi::SessionPtr GetSession() const
	{
		return m_spSession;
	}

	const std::string & GetPosture() const
	{
		return m_Posture;
	}

	bool IsConnected() const
	{
		return m_spSession->isConnected();
	}

	bool HasService( const std::string & a_serviceId ) const
	{
		if (! m_spSession->isConnected() )
			return false;
		for(size_t i=0;i<m_Services.size();++i)
			if ( m_Services[i].name() == a_serviceId ) 
				return true;
		return false;
	}

	bool IsRestPosture() const
	{
		return m_Posture == "rest" || m_Posture == "Crouch";
	}

	bool IsExtraMovementDisabled() const
	{
		return m_bExtraMovementDisabled;
	}

	bool IsAutonomousLifeStarted() const
	{
		return m_bAutonomousLifeStarted;
	}

	void DisableExtraMovement();

	//! Connect the session, may throw an exception.
	void ConnectSession();
	//! Change our current posture.
	qi::Future<bool> SetPosture( const std::string & a_PostureId, float a_fSpeed = 1.0f );
	//! Set posture id
	void SetPostureId( const std::string & a_PostureId );
	void EnableExtraMovement();
	//! Posture notifiers
	void AddPostureNotifier( Delegate<const std::string &> a_Delegate);
	bool RemovePostureNotifier( void * a_pObject );

private:
	//! Data
	std::string			m_URL;
	qi::SessionPtr		m_spSession;
	qi::AnyObject		m_Memory;
	qi::AnyObject		m_PostureChangedSub;

	std::vector<qi::ServiceInfo> 
						m_Services;
	std::string			m_Posture;
	TimerPool::ITimer::SP
						m_spReconnectTimer;
	TimerPool::ITimer::SP
						m_spDisableTimer;
	TimerPool::ITimer::SP
						m_spWaitTimer;

	bool				m_bExtraMovementDisabled;
	bool 				m_bAutonomousLifeStarted;
	float				m_ExtraMovementDisableInterval;

	DelegateList<const std::string &>
						m_PostureNotifierList;

	void Reconnect();
	void StartServices();
	qi::AnyReference OnPostureChanged(const std::vector<qi::AnyReference> & touchInfo );

	static NaoPlatform * sm_pInstance;
};

#endif