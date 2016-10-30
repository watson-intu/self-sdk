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
#include "utils/Config.h"
#include "utils/UniqueID.h"
#include "services/MergersAndAcquisitions.h"

class TestMergersAndAcquisitions : UnitTest
{
public:
    TestMergersAndAcquisitions() : UnitTest("TestMergersAndAcquisitions"),
                            m_bMergersAndAcquisitionsTested( false ),
                            m_Input( "Find me a company that sells chocolate" )
    {}

    bool m_bMergersAndAcquisitionsTested;
    std::string m_Input;


    virtual void RunTest()
    {
        Config config;
        Test(ISerializable::DeserializeFromFile("./etc/tests/unit_test_config.json", &config) != NULL);
        ThreadPool pool(1);

        MergersAndAcquisitions mergers;
        Test(mergers.Start());
        Log::Debug("TestMergersAndAcquisitions","Mergers and Acquisitions Started");

        Log::Debug("TestMergersAndAcquisitions","Testing TestMergersAndAcquisitions Response for input: %s", m_Input.c_str());
        mergers.SendRequest(m_Input, DELEGATE(TestMergersAndAcquisitions, OnMessage, const std::string &, this), 5, false );
        Spin(m_bMergersAndAcquisitionsTested, 100.0f);
        Test(m_bMergersAndAcquisitionsTested);
    }

    void OnMessage(const std::string & a_Response)
    {
        Test(a_Response.size() != 0);
        Log::Debug("TestMergersAndAcquisitions", "Received the following response: %s", a_Response.c_str());
        m_bMergersAndAcquisitionsTested = true;
    }
};

TestMergersAndAcquisitions TEST_MERGERS_AND_ACQUISITIONS;