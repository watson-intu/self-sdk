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

#include "NaoMood.h"
#include "SelfInstance.h"
#include "utils/Log.h"
#include "utils/TimerPool.h"
#include "utils/SelfException.h"

#include <stdio.h>

#ifndef _WIN32
REG_SERIALIZABLE(NaoMood);
#endif
RTTI_IMPL(NaoMood, ISensor);

bool NaoMood::OnStart()
{
    Log::Status("NaoMood", "NaoMood started");

    ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(NaoMood, ReceiveData, void *, this), NULL);
    return true;
}

bool NaoMood::OnStop()
{
    Log::Status("NaoMood", "NaoMood stopped");
    m_Memory.reset();
    qi::AnyObject faceCharacteristics = NaoPlatform::Instance()->GetSession()->service("ALFaceCharacteristics");
    faceCharacteristics.call<void>( "unsubscribe", "myApplication" );
    return true;
}

void NaoMood::ReceiveData(void * arg)
{
    try {
        DoReceiveData(arg);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoMood", "Caught Exception: %s", ex.what() );
    }
}

void NaoMood::DoReceiveData(void *)
{
    m_Memory = NaoPlatform::Instance()->GetSession()->service("ALMemory");
    qi::AnyObject faceCharacteristics = NaoPlatform::Instance()->GetSession()->service("ALFaceCharacteristics");
    faceCharacteristics.call<void>( "subscribe", "myApplication" );
    m_PersonSmiling = m_Memory.call<qi::AnyObject>( "subscriber", "FaceCharacteristics/PersonSmiling" );
    m_PersonSmiling.connect("signal", qi::AnyFunction::fromDynamicFunction( boost::bind( &NaoMood::OnPersonSmiling, this, _1 ) ) );
}

qi::AnyReference NaoMood::OnPersonSmiling( const std::vector <qi::AnyReference> &recognizeInfo )
{
    try
    {
        return DoOnRecognized(recognizeInfo);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoMood", "Caught Exception: %s", ex.what() );
    }

    return qi::AnyReference();
}

qi::AnyReference NaoMood::DoOnRecognized(const std::vector <qi::AnyReference> & args)
{
    Log::Status("NaoMood", "Received Data from Mood");
    ThreadPool::Instance()->InvokeOnMain<MoodData *>(DELEGATE(NaoMood, SendingData, MoodData * , this),
                                                      new MoodData( true ));

    return qi::AnyReference();
}

void NaoMood::SendingData(MoodData * a_pData)
{
    SendData(a_pData);
}

void NaoMood::OnPause()
{}

void NaoMood::OnResume()
{}


