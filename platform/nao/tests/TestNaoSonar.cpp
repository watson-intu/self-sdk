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
#include "../sensors/NaoSonar.h"

#ifndef _WIN32

class TestNaoSonar : UnitTest
{
public:
    //! Construction
    TestNaoSonar() : UnitTest( "TestNaoSonar" )
    {}

    virtual void RunTest()
    {
        ThreadPool pool(1);
        NaoSonar sonar;
        sonar.Subscribe( DELEGATE( TestNaoSonar, ReceiveSonar, IData *, this ) );

        Time start;
        while( (Time().GetEpochTime() - start.GetEpochTime()) < 15.0 )
        {
            // normally, this is called by SensorManager.Update()..
            tthread::this_thread::yield();
        }

        sonar.Unsubscribe( this );
    }

    void ReceiveSonar( IData * a_pData )
    {
        Log::Status( "TestNaoSonar", "Received sonar." );
    }

};

TestNaoSonar TEST_NAO_SONAR;

#endif

