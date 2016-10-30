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

#include "NaoTouch.h"
#include "SelfInstance.h"
#include "utils/ThreadPool.h"
#include "NaoPlatform.h"
#include "utils/QiHelpers.h"

REG_SERIALIZABLE( NaoTouch );
RTTI_IMPL(NaoTouch, ISensor);

bool NaoTouch::OnStart()
{
	Log::Debug("NaoTouch", "Starting up touch device");

	try {
		StartTouch();
		return true;
	}
	catch( const std::exception & ex )
	{
		Log::Error("NaoTouch", "Caught exception OnStart(): %s", ex.what() );
		return false;
	}
}

bool NaoTouch::OnStop()
{
	Log::Debug("NaoTouch", "Stopping touch device");
	if(StopTouch())
		return true;
	else
		return false;
}

void NaoTouch::StartTouch()
{
	try
	{
		DoStartTouch();
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoTouch", "Caught Exception: %s", ex.what() );
	}
}

void NaoTouch::DoStartTouch()
{
	m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");

	m_TouchSub = m_Memory.call<qi::AnyObject>("subscriber", "TouchChanged" );
	m_TouchSub.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoTouch::OnTouch, this, _1 ) ) );
}

bool NaoTouch::StopTouch()
{
	try {
		DoStopTouch();
		return true;
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoTouch", "Caught Exception: %s", ex.what() );
		return false;
	}
}

void NaoTouch::DoStopTouch()
{
	m_Memory.reset();
}

qi::AnyReference NaoTouch::OnTouch( const std::vector<qi::AnyReference> & args )
{
	try
	{
		if ( !NaoPlatform::Instance()->IsRestPosture() )
		{
			return DoOnTouch(args);
		}

	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoTouch", "Caught Exception: %s", ex.what() );
	}

	return qi::AnyReference();
}

qi::AnyReference NaoTouch::DoOnTouch(const std::vector <qi::AnyReference> & args)
{
	for (size_t i = 0; i < args.size(); ++i)
	{
		qi::AnyReference arg = args[i].content();
		qi::AnyReference touch = arg[0].content();
		std::string eventName = touch[0].content().asString();
		bool touched = touch[1].content().as<bool>();

		ThreadPool::Instance()->InvokeOnMain( DELEGATE(NaoTouch, SendData, TouchData *, this ),
											  new TouchData( eventName, touched ? 1.0f : 0.0f ) );
	}

	return qi::AnyReference();
}

void NaoTouch::OnPause()
{
	m_bPaused = true;
}

void NaoTouch::OnResume()
{
	m_bPaused = false;
}

void NaoTouch::SendData( TouchData * a_pData )
{
	if (! m_bPaused )
		ISensor::SendData( a_pData );
}



