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
#include "sensors/NaoTouch.h"

#ifndef _WIN32

class TestNaoTouch : UnitTest
{
public:
	//! Construction
	TestNaoTouch() : UnitTest("TestNaoTouch"), m_TouchTested(false)
	{}

	virtual void RunTest()
	{
		ThreadPool pool(1);
		NaoPlatform platform("tcp://192.168.1.168");

		NaoTouch touch;
		touch.Subscribe(DELEGATE(TestNaoTouch, ReceiveTouch, IData *, this));

		Time start;
		while ((Time().GetEpochTime() - start.GetEpochTime()) < 30.0 && !m_TouchTested)
		{
			ThreadPool::Instance()->ProcessMainThread();
			tthread::this_thread::yield();
		}

		touch.Unsubscribe(this);
		Test(m_TouchTested);
	}

	void ReceiveTouch(IData * a_pData)
	{
		TouchData * pTouch  = DynamicCast<TouchData>( a_pData );
		Test( pTouch != NULL );

		Log::Status("TestNaoTouch", "Received touch %s, %f", pTouch->GetSensorName().c_str(), pTouch->GetEngaged() );
		m_TouchTested = true;
	}

	bool m_TouchTested;

};

TestNaoTouch TEST_NAO_TOUCH;

#endif
