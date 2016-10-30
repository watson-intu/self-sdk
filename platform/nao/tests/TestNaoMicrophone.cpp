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
#include "sensors/NaoMicrophone.h"
#include "NaoPlatform.h"

#ifndef _WIN32

class TestNaoMicrophone : UnitTest
{
public:
	//! Construction
	TestNaoMicrophone() : UnitTest( "TestNaoMicrophone" ), m_AudioReceived( false )
	{}

	virtual void RunTest()
	{
		ThreadPool pool(1);
		NaoPlatform platform("tcp://192.168.1.168");

		NaoMicrophone mic;
		mic.Subscribe( DELEGATE( TestNaoMicrophone, ReceiveAudio, IData *, this ) );

		Time start;
		while( (Time().GetEpochTime() - start.GetEpochTime()) < 30.0 && !m_AudioReceived )
		{
			ThreadPool::Instance()->ProcessMainThread();
			tthread::this_thread::yield();
		}

		mic.Unsubscribe( this );
		Test( m_AudioReceived );
	}

	void ReceiveAudio( IData * a_pData )
	{
		Log::Status( "TestNaoAudio", "Received audio." );
		m_AudioReceived = true;
	}

	bool m_AudioReceived;

};

TestNaoMicrophone TEST_NAO_MIC;

#endif

