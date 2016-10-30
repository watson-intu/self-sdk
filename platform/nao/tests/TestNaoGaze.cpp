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
#include "sensors/NaoGaze.h"

#ifndef _WIN32

class TestNaoGaze : UnitTest
{
public:

    int m_nSubscriberCount;

    //! Construction
    TestNaoGaze() : UnitTest( "TestNaoGaze" ), m_nSubscriberCount(0)
    {}

    virtual void RunTest()
    {
        ThreadPool pool(1);
        NaoGaze gaze;
        gaze.Subscribe( DELEGATE( TestNaoGaze, ReceiveGaze, IData *, this ) );

        Spin( m_nSubscriberCount, 3, 45.0 );
        Test( m_nSubscriberCount == 3);
        gaze.Unsubscribe( this );
    }

    void ReceiveGaze( IData * a_pData )
    {
        m_nSubscriberCount += 1;
        Log::Status( "TestNaoGaze", "Received gaze event." );
    }

};

TestNaoGaze TEST_NAO_GAZE;

#endif