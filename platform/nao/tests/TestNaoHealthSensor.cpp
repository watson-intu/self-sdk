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

#include "tests/UnitTest.h"
#include "utils/Log.h"
#include "utils/Time.h"
#include "utils/ThreadPool.h"
#include "sensors/NaoHealthSensor.h"

#ifndef _WIN32

class TestNaoHealthSensor : UnitTest
{
public:
	//! Construction
	TestNaoHealthSensor() : UnitTest( "TestNaoHealthSensor" ), m_HealthChecked( false )
	{}

	virtual void RunTest()
	{
		ThreadPool pool(1);

		NaoHealthSensor healthSensor;
		healthSensor.Subscribe( DELEGATE( TestNaoHealthSensor, CheckHealth, IData *, this ) );

		Time start;
		while( (Time().GetEpochTime() - start.GetEpochTime()) < 30.0 && !m_HealthChecked )
		{
			ThreadPool::Instance()->ProcessMainThread();
			tthread::this_thread::yield();
		}

		healthSensor.Unsubscribe( this );
		Test( m_HealthChecked );
	}

	void CheckHealth( IData * a_pData )
	{
		Log::Debug( "TestNaoHealthSensor", "Checked health" );
		m_HealthChecked = true;
	}

	bool m_HealthChecked;

};

TestNaoHealthSensor TEST_NAO_HEALTH;

#endif