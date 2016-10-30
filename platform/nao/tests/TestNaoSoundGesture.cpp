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
#include "gestures/NaoSoundGesture.h"
#include "NaoPlatform.h"

class TestNaoSoundGesture : UnitTest
{
public:
	bool m_bPlayTested;
	bool m_bPlayWebTested;

	//! Construction
	TestNaoSoundGesture() : UnitTest( "TestNaoSoundGesture" ), m_bPlayTested( false ), m_bPlayWebTested( false )
	{}

	virtual void RunTest()
	{
		ThreadPool pool(1);
		NaoPlatform platform( "tcp://192.168.1.115:9559" );
		//NaoPlatform platform( "tcp://127.0.0.1:51033" );

		NaoSoundGesture gesture;

		ParamsMap params;
		params["sound"] = "/usr/share/naoqi/wav/shutdown.wav";
		Test( gesture.Execute( DELEGATE( TestNaoSoundGesture, OnGesture, const IGesture::Result &, this ), params ) );
		Spin( m_bPlayTested );
		Test( m_bPlayTested );

		params["sound"] = "http://www.wavsource.com/snds_2016-07-31_4219147316985583/sfx/applause2_x.wav";
		Test( gesture.Execute( DELEGATE( TestNaoSoundGesture, OnWebGesture, const IGesture::Result &, this ), params ) );
		Spin( m_bPlayWebTested );
		Test( m_bPlayWebTested );
	}

	void OnGesture( const IGesture::Result & )
	{
		m_bPlayTested = true;
	}

	void OnWebGesture( const IGesture::Result & )
	{
		m_bPlayWebTested = true;
	}
};

TestNaoSoundGesture TEST_NAO_SOUND_GESTURE;



