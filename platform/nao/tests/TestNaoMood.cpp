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
#include "sensors/NaoMood.h"

#ifndef _WIN32

class TestNaoMood : UnitTest
{
public:

    int m_nSubscriberCount;
    //! Construction
    TestNaoMood() : UnitTest( "TestNaoMood" ), m_nSubscriberCount(0)
    {}

    virtual void RunTest()
    {
        ThreadPool pool(1);
        NaoMood mood;
        mood.Subscribe( DELEGATE( TestNaoMood, ReceiveMood, IData *, this ) );

        Spin( m_nSubscriberCount, 1, 45.0 );
        Test( m_nSubscriberCount == 1);
        mood.Unsubscribe( this );
    }

    void ReceiveMood( IData * a_pData )
    {
        m_nSubscriberCount += 1;
        Log::Status( "TestNaoMood", "Received mood." );
    }

};

TestNaoMood TEST_NAO_MOOD;

#endif

