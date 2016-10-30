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

#include "utils/ThreadPool.h"

#include "NaoGraspGesture.h"
#include "SelfInstance.h"
#include "NaoPlatform.h"

REG_OVERRIDE_SERIALIZABLE( GraspGesture, NaoGraspGesture );
RTTI_IMPL( NaoGraspGesture, GraspGesture );


bool NaoGraspGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
    m_Callback = a_Callback;
    m_Params = a_Params;

    ThreadPool::Instance()->InvokeOnThread( VOID_DELEGATE( NaoGraspGesture, MoveThread, this ) );
    return true;
}

bool NaoGraspGesture::Abort()
{
   try
   {
       return DoAbort();
   }
   catch( const std::exception & ex )
   {
       Log::Error( "NaoGraspGesture", "Caught Exception: %s", ex.what() );
   }

    return false;
}

bool NaoGraspGesture::DoAbort()
{
    if (!m_Callback.IsValid() )
        return false;
    if (!m_MoveOp.isRunning())
        return false;

    m_Callback.Reset();		// prevent the callback
    m_MoveOp.cancel();

    qi::AnyObject motion = NaoPlatform::Instance()->GetSession()->service("ALMotion");

    return true;
}

void NaoGraspGesture::MoveThread()
{
    try
    {
        DoMoveThread();
    }
    catch( const std::exception & ex )
    {
        Log::Error( "NaoGraspGesture", "Caught Exception: %s", ex.what() );
    }
}

void NaoGraspGesture::DoMoveThread()
{
    Log::Debug( "NaoGraspGesture", "Execute move gesture, hand = %s", m_Hand.c_str() );
    NaoPlatform::Instance()->DisableExtraMovement();

    qi::AnyObject motion = NaoPlatform::Instance()->GetSession()->service("ALMotion");
    if(m_Open)
        m_MoveOp = motion.async<void>("openHand", m_Hand );
    else
        m_MoveOp = motion.async<void>("closeHand", m_Hand );

    // now invoke the main thread to notify them the move is completed..
    ThreadPool::Instance()->InvokeOnMain( VOID_DELEGATE(NaoGraspGesture, MoveDone, this ) );
}

void NaoGraspGesture::MoveDone()
{
    if ( m_Callback.IsValid() )
    {
        m_Callback( this );
        m_Callback.Reset();
    }
}


