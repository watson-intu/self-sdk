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
#include "softPwm.h"
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
    Log::Status("MoveArmJointGesture", "Starting Execute");
    if ( PushRequest( a_Callback, a_Params ) )
        ThreadPool::Instance()->InvokeOnThread(  DELEGATE( MoveArmJointGesture, AnimateThread, Request *, this ), ActiveRequest() );
    return true;
}

bool MoveArmJointGesture::Abort()
{
    Log::Status("MoveArmJointGesture", "Starting Abort");
    return false;
}

void MoveArmJointGesture::AnimateThread( Request * a_pReq )
{
    Log::Status("MoveArmJointGesture", "Starting AnimateThread");
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
    Log::Status("MoveArmJointGesture", "Starting DoAnimateThread");
    Log::Debug("MoveArmJointGesture", "in DoAnimateThread");
    // make a list of possible gestures..
    std::vector<AnimationEntry> anims;
    for(size_t i=0;i<m_Animations.size();++i)
    {
        anims.push_back( m_Animations[i] );
    }

    Log::Status("MoveArmJointGesture", "Anims.size = %d", anims.size()  );
    if ( anims.size() > 0 )
    {
        Log::Status("MoveArmJointGesture", "1");
        srand( (unsigned int)Time().GetMilliseconds() );
        const AnimationEntry & entry = anims[ rand() % anims.size() ];
        Log::Debug( "MoveArmJointGesture", "Gesture %s is running behavior %s.", m_GestureId.c_str(), entry.m_AnimId.c_str() );

        if ( !m_bWiredPi )
        {
            Log::Status("MoveArmJointGesture", "2");
            m_bWiredPi = true;
            wiringPiSetup();
        }

        int DELAY_IN_MS = 30;
        const int pwmValue = 75;
        softPwmCreate(m_PinNumber, 0, 50);
        int up;
        int down;
        for(size_t i=0;i<1;++i){
            Log::Status("MoveArmJointGesture", "3");
            for(up = 0; up <= 40; up++)
            {
                softPwmWrite (m_PinNumber, up);
                delay(DELAY_IN_MS);
            }
            for(down = up; down >= 0; down--)
            {
                softPwmWrite (m_PinNumber, down);
                delay(DELAY_IN_MS * 2);
            }
        }
        wiringPiSetup();
    }
    else
    {
        Log::Status("MoveArmJointGesture", "4");
        Log::Warning( "MoveArmJointGesture", "No valid animations found for gesture %s", m_GestureId.c_str() );
    }
}



void MoveArmJointGesture::MoveArmUp(){
        for(int i = 0; i <= 40; i++)
        {
            softPwmWrite (m_PinNumber, i);
            delay(10);
        }
    wiringPiSetup();
}

void MoveArmJointGesture::AnimateDone( Request * a_pReq )
{
    Log::Status("MoveArmJointGesture", "Starting AnimateDone");
    Log::Debug("MoveArmJointGesture", "In AnimateDone");
    Status::SP spStatus(new Status());
    SelfInstance::GetInstance()->GetBlackBoard()->AddThing(spStatus);
    // pop the request, if true is returned then start the next request..
    if ( PopRequest() )
        ThreadPool::Instance()->InvokeOnThread(DELEGATE(MoveArmJointGesture, AnimateThread, Request * , this), ActiveRequest());
}
