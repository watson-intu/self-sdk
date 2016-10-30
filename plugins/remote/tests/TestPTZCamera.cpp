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
#include "utils/ThreadPool.h"
#include "utils/Config.h"
#include "services/PTZCamera.h"

class TestPTZCamera : UnitTest 
{
public:
	//! Construction
	TestPTZCamera() : UnitTest("TestPTZCamera"),
		m_bImage(false),
		m_bCamera(false),
		m_Counter(0)
	{ }

	bool    m_bImage;
	bool    m_bCamera;
	int     m_Counter;

	virtual void RunTest()
	{
		Config config;
		Test(ISerializable::DeserializeFromFile("./etc/tests/unit_test_config.json", &config) != NULL);

		ThreadPool pool(1);
		PTZCamera * camera = new PTZCamera();
		Test( camera->Start() );
		Log::Debug("TestPTZCamera", "Instantiated Service");

		Test(camera != NULL);
		camera->GetImage(DELEGATE(TestPTZCamera, OnGetImage, const std::string &, this));
		camera->SetCameraCoordinates("left", DELEGATE(TestPTZCamera, OnCameraMovement, const std::string &, this));

		Spin(m_Counter, 2);

		Test(m_bImage);
		Test(m_bCamera);
	}

	void OnGetImage(const std::string & a_Callback)
	{
		m_Counter++;
		m_bImage = true;
	}

	void OnCameraMovement(const std::string & a_Callback)
	{
		m_Counter++;
		m_bCamera = true;
	}
};

TestPTZCamera TEST_PTZ_CAMERA;