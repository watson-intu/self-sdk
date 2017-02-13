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
#include "MoveArmJointGesture.h"
#include "SelfInstance.h"
#include "utils/ThreadPool.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Status.h"

REG_OVERRIDE_SERIALIZABLE( AnimateGesture, MoveArmJointGesture );
REG_SERIALIZABLE(MoveArmJointGesture);
RTTI_IMPL( MoveArmJointGesture, AnimateGesture );

bool MoveArmJointGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
    if ( PushRequest( a_Callback, a_Params ) )
        ThreadPool::Instance()->InvokeOnThread(  DELEGATE( MoveArmJointGesture, AnimateThread, Request *, this ), ActiveRequest() );
    return true;
}

bool MoveArmJointGesture::Abort()
{
    return false;
}

void MoveArmJointGesture::AnimateThread( Request * a_pReq )
{
    try {
        DoAnimateThread(a_pReq);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "MoveArmJointGesture", "Caught Exception: %s", ex.what() );
    }

    // now invoke the main thread to notify them the move is completed..
    ThreadPool::Instance()->InvokeOnMain( DELEGATE(MoveArmJointGesture, AnimateDone, Request *, this ), a_pReq );
}

//  The DoAnimateThread() function will check the number of gestures being posted. 
//  Using this on the RaspberryPi for the LED you should only receive a single object 
//  response. This will the check that the response is valid. If it is then, the LED will
//  flash on an off 5 times with a delay of 0.2 seconds. 
void MoveArmJointGesture::DoAnimateThread(MoveArmJointGesture::Request * a_pReq)
{
    Log::Debug("MoveArmJointGesture", "in DoAnimateThread");
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
        Log::Debug( "MoveArmJointGesture", "Gesture %s is running behavior %s.", m_GestureId.c_str(), entry.m_AnimId.c_str() );

        if ( !m_bWiredPi )
        {
            wiringPiSetup();
            pinMode(m_PinNumber, PWM_OUTPUT);
            m_bWiredPi = true;
        }

        for(size_t i=0;i<5;++i)
        {
            pwmWrite(m_PinNumber, 200);
            delay(200);
            pwmWrite(m_PinNumber, 500);
            delay(200);
        }
    }
    else
    {
        Log::Warning( "MoveArmJointGesture", "No valid animations found for gesture %s", m_GestureId.c_str() );
    }
}

void MoveArmJointGesture::AnimateDone( Request * a_pReq )
{
    Log::Debug("MoveArmJointGesture", "In AnimateDone");
    Status::SP spStatus(new Status());
    SelfInstance::GetInstance()->GetBlackBoard()->AddThing(spStatus);
    // pop the request, if true is returned then start the next request..
    if ( PopRequest() )
        ThreadPool::Instance()->InvokeOnThread(DELEGATE(MoveArmJointGesture, AnimateThread, Request * , this), ActiveRequest());
}
