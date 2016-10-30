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
#include "sensors/Nao3DCamera.h"

#ifndef _WIN32

class TestNao3DCamera : UnitTest
{
public:
    //! Construction
    TestNao3DCamera() : UnitTest( "TestNao3DCamera" )
    {}

    virtual void RunTest()
    {
        ThreadPool pool(1);
        Nao3DCamera video;
        video.Subscribe( DELEGATE( TestNao3DCamera, ReceiveVideo, IData *, this ) );

        Time start;
        while( (Time().GetEpochTime() - start.GetEpochTime()) < 10.0 )
        {
			ThreadPool::Instance()->ProcessMainThread();
			tthread::this_thread::yield();
        }

        video.Unsubscribe( this );
    }

    void ReceiveVideo( IData * a_pData )
    {
        Log::Status( "TestNaoVideo", "Received video." );
    }

};

TestNao3DCamera TEST_NAO_3D_CAMERA;

#endif

