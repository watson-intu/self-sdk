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

#include "NaoPlatform.h"
#include "NaoMoveGesture.h"
#include "SelfInstance.h"
#include "utils/ThreadPool.h"

REG_OVERRIDE_SERIALIZABLE( MoveGesture, NaoMoveGesture );
RTTI_IMPL( NaoMoveGesture, MoveGesture );


bool NaoMoveGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	if ( PushRequest( a_Callback, a_Params ) )
		ThreadPool::Instance()->InvokeOnThread( VOID_DELEGATE( NaoMoveGesture, MoveThread, this ) );
	return true;
}

bool NaoMoveGesture::Abort()
{
	try {
		return DoAbort();
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoMoveGesture", "Caught Exception: %s", ex.what() );
	}

	return false;
}

bool NaoMoveGesture::DoAbort()
{
	if (! HaveRequests() )
		return false;

	m_MoveOp.cancel();

	qi::AnyObject motion = NaoPlatform::Instance()->GetSession()->service("ALMotion");
	motion.async<void>( "stopMove" );

	PopAllRequests();
	return true;
}

void NaoMoveGesture::MoveThread()
{
	try {
		DoMoveThread();
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoMoveGesture", "Caught Exception: %s", ex.what() );
	}
}

void NaoMoveGesture::DoMoveThread()
{
	Log::Debug( "NaoMoveGesture", "Execute move gesture, x = %f, y = %f, z = %f", m_fX, m_fY, m_fZ );
	NaoPlatform::Instance()->DisableExtraMovement();

	qi::AnyObject motion = NaoPlatform::Instance()->GetSession()->service("ALMotion");
	m_MoveOp = motion.async<void>("moveTo", m_fY, m_fY, m_fZ );

	// now invoke the main thread to notify them the move is completed..
	ThreadPool::Instance()->InvokeOnMain( VOID_DELEGATE(NaoMoveGesture, MoveDone, this ) );
}

void NaoMoveGesture::MoveDone()
{
	// pop the request, if true is returned then start the next request..
	if ( PopRequest() )
		ThreadPool::Instance()->InvokeOnThread( VOID_DELEGATE( NaoMoveGesture, MoveThread, this ) );
}
