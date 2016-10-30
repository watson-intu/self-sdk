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
#include "SelfInstance.h"
#include "services/FacialRecognition.h"

#include <fstream>

class TestFacialRecognition : UnitTest {
public:
    //! Construction
    TestFacialRecognition() : UnitTest("TestFacialRecognition"),
                              m_bDetectFacesTested(false),
                              m_bIdentifyFacesTested(false),
                              m_bPeopleTested(false) { }

    bool m_bPeopleTested;
    bool m_bDetectFacesTested;
    bool m_bIdentifyFacesTested;

    virtual void RunTest()
    {
        // read in all the file data..
        std::ifstream input("./etc/tests/AlchemyTest.jpg", std::ios::in | std::ios::binary);
        Test(input.is_open());
        std::string imageData;
        imageData.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
        input.close();



        SelfInstance instance( "./etc/profile/" );
        Test( instance.Start() );
        Log::Debug("TestFacialRecognition", "Started Instance");

        FacialRecognition * facialRecognition = instance.GetService<FacialRecognition>();
        Log::Debug("TestFacialRecognition", "Instantiated Service");

        Test(facialRecognition != NULL);
        facialRecognition->GetAllPeople(DELEGATE(TestFacialRecognition, OnGetPeople, People *, this));
        facialRecognition->DetectFace(imageData,
                            DELEGATE(TestFacialRecognition, OnDetectFaces, Detect *, this) );
        facialRecognition->IdentifyFaces(imageData,
                              DELEGATE(TestFacialRecognition, OnIdentifyFaces,Identify *, this) );

        Time start;
        while ((Time().GetEpochTime() - start.GetEpochTime()) < 30.0 && (!m_bDetectFacesTested || !m_bIdentifyFacesTested) )
        {
            ThreadPool::Instance()->ProcessMainThread();
            tthread::this_thread::yield();
        }

        Test(m_bPeopleTested);
        Test(m_bDetectFacesTested);
        Test(m_bIdentifyFacesTested);
    }

    void OnGetPeople(People * a_pPeople)
    {
        Test(a_pPeople != NULL);
        for (size_t i = 0; i < a_pPeople->m_People.size(); ++i)
        {
            const Face &f = a_pPeople->m_People[i];
            Log::Status("TestFacialRecognition", "Received person: %s", f.m_Name.c_str());
        }
        m_bPeopleTested = true;
    }

    void OnDetectFaces(Detect * a_pDetect)
    {
        Test(a_pDetect != NULL);
        Log::Status("TestFacialRecognition", "Received face id: %s", a_pDetect->m_FaceId.c_str());
        m_bDetectFacesTested = true;
    }

    void OnIdentifyFaces(Identify * a_pIdentify)
    {
        Test(a_pIdentify != NULL);
        Log::Status("TestFacialRecognition", "Received Name (with confidence): %s(%f)", a_pIdentify->m_Name.c_str(), a_pIdentify->m_Confidence);
        m_bIdentifyFacesTested = true;
    }
};

TestFacialRecognition TEST_FACIAL_RECOGNITION;