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

#include "NaoPlatform.h"

#include "utils/QiHelpers.h"
#include "utils/TimerPool.h"
#include "tinythread++/tinythread.h"

NaoPlatform * NaoPlatform::sm_pInstance = NULL;

NaoPlatform * NaoPlatform::Instance()
{
	static tthread::mutex lock;
	tthread::lock_guard<tthread::mutex> guard( lock );
	if ( sm_pInstance == NULL )
		new NaoPlatform();

	return sm_pInstance;
}

NaoPlatform::NaoPlatform( const std::string & a_URL /*= ""*/ ) 
	: m_spSession( new qi::Session() ), m_URL( a_URL ), m_bExtraMovementDisabled( false ), m_bAutonomousLifeStarted( false ), m_ExtraMovementDisableInterval( 60.0f )
{
	sm_pInstance = this;

	if ( m_URL.size() == 0 )
	{
		SelfInstance * pInstance = SelfInstance::GetInstance();
		if ( pInstance == NULL )
			m_URL = "tcp://127.0.0.1";
		else
			m_URL = pInstance->GetRobotUrl();
	}

	// start a timer to check for a connection every 5 seconds..
	SelfInstance * pSelf = SelfInstance::GetInstance();
	if ( pSelf != NULL )
	{
		m_spReconnectTimer = pSelf->GetTimerPool()->StartTimer( 
			VOID_DELEGATE( NaoPlatform, Reconnect, this ), 5.0f, true, true );
	}

	// go ahead and try to connect now..
	Reconnect();
}

NaoPlatform::~NaoPlatform()
{}

void NaoPlatform::ConnectSession()
{
	qi::Future<void> conn = m_spSession->connect( m_URL );
	conn.wait();

	if ( m_spSession->isConnected() )
	{
		m_Services = m_spSession->services().value();
		for( size_t i=0;i<m_Services.size();++i)
			Log::Debug( "NaoPlatform", "Service Found: %s", m_Services[i].name().c_str() );

		StartServices();

		if ( HasService( "ALTabletService" ) )
		{
			SelfInstance * pInstance = SelfInstance::GetInstance();
			if ( pInstance != NULL )
			{
				const std::string & logo = pInstance->GetLogoUrl();
				if ( logo.size() > 0 )
				{
					Log::Debug( "NaoPlatform", "Setting logo: %s", logo.c_str() );
					qi::AnyObject tablet = m_spSession->service("ALTabletService" );
					bool displayLogo = tablet.call<bool>( "preLoadImage", logo );
					Log::Debug( "NaoPlatform", "Loading logo successful/failed: %d", displayLogo );
					displayLogo = tablet.call<bool>("showImageNoCache", logo);
					Log::Debug("NaoPlatform", "Displaying logo successful/failed: %d", displayLogo);
				}
			}
		}
	}
}

void NaoPlatform::Reconnect()
{
	if (! m_spSession->isConnected() )
	{
		try {
			ConnectSession();
		}
		catch( const std::exception & ex )
		{
			Log::Error( "NaoPlatform", "Caught Exception: %s", ex.what() );
		}
	}
}

qi::Future<bool> NaoPlatform::SetPosture( const std::string & a_PostureId, float a_fSpeed/* = 1.0f*/ )
{
	qi::AnyObject motion = m_spSession->service("ALMotion");
	motion.call<void>("stiffnessInterpolation", "body", 1.0, 1.0);

	qi::AnyObject posture = m_spSession->service( "ALRobotPosture" );

	qi::Future<bool> f = posture.async<bool>( "goToPosture", a_PostureId, a_fSpeed );

	SetPostureId( a_PostureId );

	return f;
}

void NaoPlatform::SetPostureId( const std::string & a_PostureId )
{
	if (m_Posture != a_PostureId)
	{
		m_Posture = a_PostureId;
		m_PostureNotifierList.Invoke( a_PostureId );
	}
}

void NaoPlatform::AddPostureNotifier( Delegate<const std::string &> a_Delegate)
{
	m_PostureNotifierList.Add( a_Delegate );
}

bool NaoPlatform::RemovePostureNotifier( void * a_pObject )
{
	return m_PostureNotifierList.Remove( a_pObject );
}

void NaoPlatform::EnableExtraMovement()
{
	Log::Status("NaoPlatform", "enabling extra movement for the nao platform!");

	if ( HasService( "ALMotion" ) )
	{
		qi::AnyObject motion = m_spSession->service( "ALMotion" );
		motion.call<void>( "setBreathEnabled", "Body", true );
		motion.call<void>( "setBreathEnabled", "Arms", true );

		qi::AnyObject aware = m_spSession->service( "ALBasicAwareness" );
		aware.call<void>( "startAwareness" );
	}

	m_bExtraMovementDisabled = false;
}

void NaoPlatform::DisableExtraMovement()
{
	Log::Status("NaoPlatform", "Disabling extra nao movement!");

	SelfInstance * pSelf = SelfInstance::GetInstance();
	if ( pSelf != NULL )
	{
		m_bExtraMovementDisabled = true;

		if ( HasService( "ALMotion" ) )
		{
			qi::AnyObject motion = m_spSession->service( "ALMotion" );
			motion.call<void>( "setBreathEnabled", "Body", false );
			motion.call<void>( "setBreathEnabled", "Arms", false );

			qi::AnyObject aware = m_spSession->service( "ALBasicAwareness" );
			aware.call<void>( "stopAwareness" );
		}

		m_spDisableTimer = pSelf->GetTimerPool()->StartTimer(
				VOID_DELEGATE(NaoPlatform, EnableExtraMovement, this), m_ExtraMovementDisableInterval, true, false);
	}
}

qi::AnyReference NaoPlatform::OnPostureChanged(const std::vector<qi::AnyReference> & args )
{
	Log::DebugLow( "NaoPlatform", "OnPostureChanged() invoked." );
	for(size_t i=0;i<args.size();++i)
		Log::DebugLow( "NaoPlatform", "arg[%u] = %s", i, QiHelpers::DumpValue( args[i] ).c_str() );

	std::string posture = args[0].content().asString();
	if ( posture == "StandInit" || posture == "StandZero" )
		posture = "Stand";

	Log::Status( "NaoPlatform", "OnPostureChanged() event, posture = %s", 
		posture.c_str() );
	SetPostureId( posture );

	return qi::AnyReference();

}

void NaoPlatform::StartServices()
{
	if ( HasService( "ALAutonomousLife" ) )
	{
		Log::Debug( "NaoPlatform", "Turning off autonomous life." );
		qi::AnyObject life = m_spSession->service( "ALAutonomousLife" );
		try
		{
			life.call<void>( "setState", "disabled" );
			if ( HasService( "ALMotion" ) )
			{
				Log::Debug( "NaoPlatform", "Turning on breathing motion." );
				qi::AnyObject motion = m_spSession->service( "ALMotion" );
				motion.call<void>( "wakeUp" );
			}

			if ( HasService( "ALBasicAwareness" ) )
			{
				Log::Debug( "NaoPlatform", "starting awareness." );
				qi::AnyObject aware = m_spSession->service( "ALBasicAwareness" );
				aware.call<void>( "startAwareness" );
			}

			if ( HasService( "ALMemory" ) )
			{
				m_Memory = m_spSession->service("ALMemory");
				m_PostureChangedSub = m_Memory.call<qi::AnyObject>("subscriber", "PostureChanged" );
				m_PostureChangedSub.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoPlatform::OnPostureChanged, this, _1 ) ) );
			}

			if (! IsExtraMovementDisabled() )
				EnableExtraMovement();
			m_bAutonomousLifeStarted = true;
		}
		catch (  const std::exception & ex )
		{
			Log::Error("NaoPlatform", "Received the following error trying to set state for autonomous life: %s", ex.what());
			Log::Error("NaoPlatform", "Got state from Autonomous life %s, retrying...", life.call<std::string>("getState").c_str());
			m_spWaitTimer = TimerPool::Instance()->StartTimer(
					VOID_DELEGATE(NaoPlatform, StartServices, this), 10.0f, true, false);
		}
	}
}