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
#include "gestures/NaoVolumeGesture.h"
#include "utils/ThreadPool.h"

class TestNaoVolume : UnitTest
{
public:
	bool m_bGestureTested;

	//! Construction
	TestNaoVolume() : UnitTest( "TestNaoVolume" )
	{}

	virtual void RunTest()
	{
		ThreadPool pool(1);

		NaoVolumeGesture gesture;

		ParamsMap params;
		params["m_fChange"] = -5.0;
		Test( gesture.Execute( DELEGATE( TestNaoVolume, OnGesture, const IGesture::Result &, this ), params ) );
	}

	void OnGesture( const IGesture::Result & )
	{
		m_bGestureTested = true;
	}
};

TestNaoVolume TEST_NAO_VOLUME;
