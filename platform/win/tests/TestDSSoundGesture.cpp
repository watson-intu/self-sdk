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
#include "gestures/DSSoundGesture.h"

class TestDSSoundGesture : UnitTest
{
public:
	bool m_bGestureTested = false;

	//! Construction
	TestDSSoundGesture() : UnitTest("TestDSSoundGesture"), m_bGestureTested(false)
	{}

	virtual void RunTest()
	{
		ThreadPool pool(1);

		ParamsMap params;
		params["sound"] = "./etc/tests/tos-redalert.wav";

		DSSoundGesture gesture;
		Test(gesture.Execute(DELEGATE(TestDSSoundGesture, OnGestureComplete, const IGesture::Result &, this), params));
		Spin(m_bGestureTested);
		Test(m_bGestureTested);
	}

	void OnGestureComplete( const IGesture::Result & )
	{
		m_bGestureTested = true;
	}
};

TestDSSoundGesture TEST_DS_SOUND_GESTURE;
