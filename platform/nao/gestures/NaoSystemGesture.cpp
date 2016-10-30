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
#include "NaoSystemGesture.h"
#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Health.h"
#include "utils/ThreadPool.h"

REG_OVERRIDE_SERIALIZABLE( SystemGesture, NaoSystemGesture );
RTTI_IMPL( NaoSystemGesture, SystemGesture );


bool NaoSystemGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	if ( PushRequest( a_Callback, a_Params ) )
		ThreadPool::Instance()->InvokeOnThread(  DELEGATE( NaoSystemGesture, SystemThread, Request *, this ), ActiveRequest() );
	return true;
}

void NaoSystemGesture::SystemThread( Request * a_pReq )
{
	try {
		DoSystemThread(a_pReq);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoSystemGesture", "Caught Exception: %s", ex.what() );
	}

	ThreadPool::Instance()->InvokeOnMain( DELEGATE(NaoSystemGesture, SystemDone, Request *, this ), a_pReq );
}

void NaoSystemGesture::DoSystemThread( Request * a_pReq )
{
	qi::AnyObject system = NaoPlatform::Instance()->GetSession()->service("ALSystem");
	system.call<void>( m_SystemId );
}

void NaoSystemGesture::SystemDone( Request * a_pReq )
{
	// DISABLING: Gestures should NOT be putting objects onto the blackboard directly!
	//SelfInstance * pInstance = SelfInstance::GetInstance();
	//if ( pInstance != NULL )
	//	pInstance->GetBlackBoard()->AddThing(Health::SP(new Health("system", m_SystemId, 0.0, false)));

	// pop the request, if true is returned then start the next request..
	if ( PopRequest() )
		ThreadPool::Instance()->InvokeOnThread( DELEGATE( NaoSystemGesture, SystemThread, Request *, this ), ActiveRequest() );
}

bool NaoSystemGesture::Abort()
{
	return false;
}
