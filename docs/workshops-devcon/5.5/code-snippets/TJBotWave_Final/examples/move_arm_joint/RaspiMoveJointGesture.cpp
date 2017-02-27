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
#include "RaspiMoveJointGesture.h"
#include "SelfInstance.h"
#include "utils/ThreadPool.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Status.h"

REG_OVERRIDE_SERIALIZABLE( MoveJointGesture, RaspiMoveJointGesture );
REG_SERIALIZABLE(RaspiMoveJointGesture);
RTTI_IMPL( RaspiMoveJointGesture, MoveJointGesture );

int RaspiMoveJointGesture::m_CurrentPWMArmState = 0;

bool RaspiMoveJointGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
    if ( PushRequest( a_Callback, a_Params ) )
        ThreadPool::Instance()->InvokeOnThread(  DELEGATE( RaspiMoveJointGesture, MoveJointThread, Request *, this ), ActiveRequest() );
    return true;
}

bool RaspiMoveJointGesture::Abort()
{
    return false;
}

void RaspiMoveJointGesture::MoveJointThread( Request * a_pReq )
{
    try {
        if ( !m_bWiredPi )
        {
            m_bWiredPi = true;
            wiringPiSetup();
            softPwmCreate(m_PinNumber, 0, 50);
        }
        MoveArm();
    }
    catch( const std::exception & ex )
    {
        Log::Error( "RaspiMoveJointGesture", "Caught Exception: %s", ex.what() );
    }

    // now invoke the main thread to notify them the move is completed..
    ThreadPool::Instance()->InvokeOnMain( DELEGATE(RaspiMoveJointGesture, MoveJointDone, Request *, this ), a_pReq );
}

void RaspiMoveJointGesture::MoveArm()
{
    if(PWMValue() > m_CurrentPWMArmState) //move arm upward
    {
        Log::Debug("RaspiMoveJointGesture", "Current Raspi Arm State %d", m_CurrentPWMArmState );
        Log::Debug("RaspiMoveJointGesture", "Moving Raspi Arm Upward to %d", PWMValue() );
        for(int i = m_CurrentPWMArmState ; i <= PWMValue(); i++)
        {
            softPwmWrite (m_PinNumber, i);
            delay(10);
            m_CurrentPWMArmState = PWMValue();
        }
        wiringPiSetup();
    }
    if(PWMValue() < m_CurrentPWMArmState) //move arm downward
    {
        Log::Debug("RaspiMoveJointGesture", "Current Raspi Arm State %d", m_CurrentPWMArmState );
        Log::Debug("RaspiMoveJointGesture", "Moving Raspi Arm Downward to %d", PWMValue() );
        for(int i = m_CurrentPWMArmState; i >= PWMValue() ; i--)
        {
            softPwmWrite (m_PinNumber, i);
            delay(10);
            m_CurrentPWMArmState = PWMValue();
        }
        wiringPiSetup();
    }

}

int RaspiMoveJointGesture::PWMValue(){
    return (  ( abs(m_fAngles.front()) % 360   )/360.0 ) * 100;
}

void RaspiMoveJointGesture::MoveJointDone( Request * a_pReq )
{
    Status::SP spStatus(new Status());
    SelfInstance::GetInstance()->GetBlackBoard()->AddThing(spStatus);
    if ( PopRequest() )    // pop the request, if true is returned then start the next request..
    {
        ThreadPool::Instance()->InvokeOnThread(DELEGATE(RaspiMoveJointGesture, MoveJointThread, Request * , this), ActiveRequest());
    }
}
