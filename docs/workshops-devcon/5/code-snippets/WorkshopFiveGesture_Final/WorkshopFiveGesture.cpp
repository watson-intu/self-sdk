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

#include "wiringPi.h"
#include "WorkshopFiveGesture.h"
#include "SelfInstance.h"
#include "utils/ThreadPool.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Status.h"

REG_OVERRIDE_SERIALIZABLE( AnimateGesture, WorkshopFiveGesture );
REG_SERIALIZABLE(WorkshopFiveGesture);
RTTI_IMPL( WorkshopFiveGesture, AnimateGesture );

bool WorkshopFiveGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
    if ( PushRequest( a_Callback, a_Params ) )
        ThreadPool::Instance()->InvokeOnThread(  DELEGATE( WorkshopFiveGesture, AnimateThread, Request *, this ), ActiveRequest() );
    return true;
}

bool WorkshopFiveGesture::Abort()
{
    return false;
}

void WorkshopFiveGesture::AnimateThread( Request * a_pReq )
{
    try {
        DoAnimateThread(a_pReq);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "WorkshopFiveGesture", "Caught Exception: %s", ex.what() );
    }

    // now invoke the main thread to notify them the move is completed..
    ThreadPool::Instance()->InvokeOnMain( DELEGATE(WorkshopFiveGesture, AnimateDone, Request *, this ), a_pReq );
}

//  The DoAnimateThread() function will check the number of gestures being posted. 
//  Using this on the RaspberryPi for the LED you should only receive a single object 
//  response. This will the check that the response is valid. If it is then, the LED will
//  flash on an off 5 times with a delay of 0.2 seconds. 
void WorkshopFiveGesture::DoAnimateThread(WorkshopFiveGesture::Request * a_pReq)
{
    Log::Debug("WorkshopFiveGesture", "in DoAnimateThread");
    // make a list of possible gestures..
    std::vector<AnimationEntry> anims;
    for(size_t i=0;i<m_Animations.size();++i)
    {
        anims.push_back( m_Animations[i] );
    }

    if ( anims.size() > 0 )
    {
        srand( (unsigned int)Time().GetMilliseconds() );
        const AnimationEntry & entry = anims[ rand() % anims.size() ];
        Log::Debug( "WorkshopFiveGesture", "Gesture %s is running behavior %s.", m_GestureId.c_str(), entry.m_AnimId.c_str() );

        if ( !m_bWiredPi )
        {
            wiringPiSetup();
            pinMode(m_PinNumber, OUTPUT);
            m_bWiredPi = true;
        }
        for(size_t i=0;i<5;++i)
        {
            digitalWrite(m_PinNumber, HIGH);
            delay(200);
            digitalWrite(m_PinNumber, LOW);
            delay(200);
        }
    }
    else
    {
        Log::Warning( "WorkshopFiveGesture", "No valid animations found for gesture %s", m_GestureId.c_str() );
    }
}

void WorkshopFiveGesture::AnimateDone( Request * a_pReq )
{
    Log::Debug("WorkshopFiveGesture", "In AnimateDone");
    Status::SP spStatus(new Status());
    SelfInstance::GetInstance()->GetBlackBoard()->AddThing(spStatus);
    // pop the request, if true is returned then start the next request..
    if ( PopRequest() )
        ThreadPool::Instance()->InvokeOnThread(DELEGATE(WorkshopFiveGesture, AnimateThread, Request * , this), ActiveRequest());
}
