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

#define _CRT_SECURE_NO_WARNINGS

#include "tests/UnitTest.h"
#include "utils/ThreadPool.h"
#include "sensors/RemoteMicrophone.h"

#include <stdio.h>

class TestRemoteMicrophone : UnitTest
{
public:
	//! Construction
	TestRemoteMicrophone() : UnitTest("TestRemoteMicrophone"),
		m_Counter(0)
	{ }

	int     m_Counter;

	virtual void RunTest()
	{
		ThreadPool pool(1);

		FILE * fp = fopen("./etc/tests/test.ogg", "rb");
		Test(fp != NULL);

		fseek(fp, 0, SEEK_END);
		size_t fileLen = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		ITopics::Payload payload;
		payload.m_Data.resize(fileLen);
		fread(&payload.m_Data[0], 1, fileLen, fp);
		fclose(fp);

		RemoteMicrophone mic;
		Test(mic.OnStart());
		mic.Subscribe(DELEGATE(TestRemoteMicrophone, OnAudioData, IData *, this));
		mic.OnRemoteAudio(payload);
		Spin(m_Counter, 1);

		Test(mic.Unsubscribe(this));
	}

	void OnAudioData(IData * pData)
	{
		AudioData * pAudio = DynamicCast<AudioData>(pData);
		Test(pAudio != NULL);
		Test(pAudio->GetWaveData().size() > 0);
		Test(pAudio->GetFrequency() > 0);

		Sound sound;
		sound.InitializeSound(pAudio->GetFrequency(), pAudio->GetChannels(), pAudio->GetBPS(), pAudio->GetWaveData());
		sound.SaveToFile("./etc/tests/test.wav");

		m_Counter++;
	}
};

TestRemoteMicrophone TEST_REMOTE_MICROPHONE;