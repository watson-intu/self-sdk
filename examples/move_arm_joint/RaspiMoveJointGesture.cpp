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

//REG_OVERRIDE_SERIALIZABLE( AnimateGesture, MoveArmJointGesture );
//REG_SERIALIZABLE(MoveArmJointGesture);
//RTTI_IMPL( MoveArmJointGesture, AnimateGesture );

REG_OVERRIDE_SERIALIZABLE( MoveJointGesture, RaspiMoveJointGesture );
REG_SERIALIZABLE(RaspiMoveJointGesture);
RTTI_IMPL( RaspiMoveJointGesture, MoveJointGesture );


bool RaspiMoveJointGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
    Log::Status("RaspiMoveJointGesture", "Starting Execute");
    if ( PushRequest( a_Callback, a_Params ) )
        ThreadPool::Instance()->InvokeOnThread(  DELEGATE( RaspiMoveJointGesture, AnimateThread, Request *, this ), ActiveRequest() );
    return true;
}

bool RaspiMoveJointGesture::Abort()
{
    Log::Status("RaspiMoveJointGesture", "Starting Abort");
    return false;
}


//void RaspiMoveJointGesture::Serialize(Json::Value & json)
//{
//    IGesture::Serialize( json );
//
//    json["m_fStiffness"] = m_fStiffness;
//    json["m_bAbsolute"] = m_bAbsolute;
//    json["m_GestureId"] = m_GestureId;
//
//    SerializeVector("m_JointNames", m_JointNames, json);
//    SerializeVector("m_fAngles", m_fAngles, json);
//    SerializeVector("m_fSpeeds", m_fSpeeds, json);
//}
//
//void RaspiMoveJointGesture::Deserialize(const Json::Value & json)
//{
//    IGesture::Deserialize( json );
//
//    m_fStiffness = json["m_fStiffness"].asFloat();
//    m_bAbsolute = json["m_bAbsolute"].asBool();
//    m_GestureId = json["m_GestureId"].asString();
//
//    DeserializeVector("m_JointNames", json, m_JointNames);
//    DeserializeVector("m_fAngles", json, m_fAngles);
//    DeserializeVector("m_fSpeeds", json, m_fSpeeds);
//
//    if(m_JointNames.size() == 0)
//        m_JointNames.push_back(json["m_JointName"].asString());
//    if(m_fAngles.size() == 0)
//        m_fAngles.push_back(json["m_fAngle"].asFloat());
//    if(m_fSpeeds.size() == 0)
//        m_fSpeeds.push_back(json["m_fSpeed"].asFloat());
//}


void RaspiMoveJointGesture::AnimateThread( Request * a_pReq )
{
    Log::Status("RaspiMoveJointGesture", "Starting AnimateThread");
    try {
        DoAnimateThread(a_pReq);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "RaspiMoveJointGesture", "Caught Exception: %s", ex.what() );
    }

    // now invoke the main thread to notify them the move is completed..
    ThreadPool::Instance()->InvokeOnMain( DELEGATE(RaspiMoveJointGesture, AnimateDone, Request *, this ), a_pReq );
}

//  The DoAnimateThread() function will check the number of gestures being posted. 
//  Using this on the RaspberryPi for the LED you should only receive a single object 
//  response. This will the check that the response is valid. If it is then, the LED will
//  flash on an off 5 times with a delay of 0.2 seconds. 
void RaspiMoveJointGesture::DoAnimateThread(RaspiMoveJointGesture::Request * a_pReq)
{
    Log::Status("RaspiMoveJointGesture", "Starting DoAnimateThread");
    // make a list of possible gestures..
        srand( (unsigned int)Time().GetMilliseconds() );
        if ( !m_bWiredPi )
        {
            m_bWiredPi = true;
            wiringPiSetup();
            softPwmCreate(m_PinNumber, 0, 50);
        }
        Log::Status("RaspiMoveJointGesture", "m_CurrentPWMArmState %d", m_CurrentPWMArmState );
        Log::Status("RaspiMoveJointGesture", "PWMVALUE %d", PWMValue() );
        MoveArm();

//        if(m_GestureId == "r_hand_raise" || m_GestureId == "l_hand_raise")
//        {
//            Log::Status("RaspiMoveJointGesture", "Raising Hand");
//            MoveArmUp();
//        }
//
//        if(m_GestureId == "r_hand_descend" || m_GestureId == "l_hand_descend")
//        {
//            Log::Status("RaspiMoveJointGesture", "Lowering Hand");
//            MoveArmDown();
//        }

//        for(size_t i=0;i<1;++i){
//            MoveArmUp();
//            MoveArmDown();
//        }

}

//(0-360/360) * 100

int RaspiMoveJointGesture::PWMValue(){
    return ((m_fAngles.front())/360.0 ) * 100;
}


void RaspiMoveJointGesture::MoveArm()
{
    if(PWMValue() > m_CurrentPWMArmState) //move arm upward
    {
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
        for(int i = m_CurrentPWMArmState; i >= PWMValue() ; i--)
        {
            softPwmWrite (m_PinNumber, i);
            delay(10);
            m_CurrentPWMArmState = PWMValue();
        }
        wiringPiSetup();
    }

}



//void RaspiMoveJointGesture::MoveArmUp()
//{
//    for(int i = m_ArmLowPoint ; i <= PWMValue(); i++)
//    {
//        softPwmWrite (m_PinNumber, i);
//        delay(10);
//    }
//    wiringPiSetup();
//}
//
//void RaspiMoveJointGesture::MoveArmDown(){
//    for(int i = m_ArmHighPoint; i >= m_ArmLowPoint ; i--)
//    {
//        softPwmWrite (m_PinNumber, i);
//        delay(10);
//    }
//    wiringPiSetup();
//}

void RaspiMoveJointGesture::AnimateDone( Request * a_pReq )
{
    Log::Status("RaspiMoveJointGesture", "Starting AnimateDone");
    Log::Debug("RaspiMoveJointGesture", "In AnimateDone");
    Status::SP spStatus(new Status());
    SelfInstance::GetInstance()->GetBlackBoard()->AddThing(spStatus);
    // pop the request, if true is returned then start the next request..
    if ( PopRequest() )
        ThreadPool::Instance()->InvokeOnThread(DELEGATE(RaspiMoveJointGesture, AnimateThread, Request * , this), ActiveRequest());
}
