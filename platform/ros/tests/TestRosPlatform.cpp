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

#include "RosPlatform.h"
#include "gestures/RosMoveJointGesture.h"

#ifndef _WIN32

class TestRosPlatform : UnitTest
{
public:

    bool m_bFinished;
    //! Construction
    TestRosPlatform() : UnitTest( "TestRosPlatform" ),  m_bFinished( false )
    {}

    virtual void RunTest()
    {
        Log::Debug("TestRosPlatform", "Started TestRosPlatform Unit Test");
        ThreadPool pool(1);
        RosPlatform platform( "ws://192.168.1.89:9090" );

        ParamsMap params;
        RosMoveJointGesture gesture;
        gesture.Execute( DELEGATE( TestRosPlatform, OnGesture, IGesture *, this ), params );
        Time start;
        while( (Time().GetEpochTime() - start.GetEpochTime()) < 10.0 )
        {
            ThreadPool::Instance()->ProcessMainThread();
            tthread::this_thread::yield();
        }
        Log::Debug("TestRosPlatform", "Finished TestRosPlatform Unit Test");
    }

    void OnGesture( IGesture * a_pGesture )
    {
        m_bFinished = true;
    }
};

TestRosPlatform TEST_NAO_PLATFORM;

#endif
