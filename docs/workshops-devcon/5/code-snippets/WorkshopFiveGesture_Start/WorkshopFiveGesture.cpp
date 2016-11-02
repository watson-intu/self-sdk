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

void WorkshopFiveGesture::DoAnimateThread(WorkshopFiveGesture::Request * a_pReq)
{
    Log::Debug("WorkshopFiveGesture", "in DoAnimateThread");
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
