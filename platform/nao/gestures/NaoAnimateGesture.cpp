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
#include "NaoAnimateGesture.h"
#include "SelfInstance.h"
#include "utils/ThreadPool.h"

REG_OVERRIDE_SERIALIZABLE( AnimateGesture, NaoAnimateGesture );
REG_SERIALIZABLE(NaoAnimateGesture);
RTTI_IMPL( NaoAnimateGesture, AnimateGesture );

bool NaoAnimateGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	if ( PushRequest( a_Callback, a_Params ) )
		ThreadPool::Instance()->InvokeOnThread(  DELEGATE( NaoAnimateGesture, AnimateThread, Request *, this ), ActiveRequest() );
	return true;
}

bool NaoAnimateGesture::Abort()
{
	return false;
}

void NaoAnimateGesture::AnimateThread( Request * a_pReq )
{
	try {
		DoAnimateThread(a_pReq);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoAnimateGesture", "Caught Exception: %s", ex.what() );
	}

	//// now invoke the main thread to notify them the move is completed..
	ThreadPool::Instance()->InvokeOnMain( DELEGATE(NaoAnimateGesture, AnimateDone, Request *, this ), a_pReq );
}

void NaoAnimateGesture::DoAnimateThread(::NaoAnimateGesture::Request * a_pReq)
{
	NaoPlatform * pPlatform = NaoPlatform::Instance();
	qi::AnyObject behavior = pPlatform->GetSession()->service("ALBehaviorManager");

	// make a list of possible gestures..
	std::vector<AnimationEntry> anims;
	for(size_t i=0;i<m_Animations.size();++i)
	{
		if ( m_Animations[i].m_RequiredPosture.size() == 0 || m_Animations[i].m_RequiredPosture == pPlatform->GetPosture() )
			anims.push_back( m_Animations[i] );
	}

	if ( anims.size() > 0 )
	{
		srand( (unsigned int)Time().GetMilliseconds() );
		const AnimationEntry & entry = anims[ rand() % anims.size() ];
		Log::Debug( "NaoAnimateGesture", "Gesture %s is running behavior %s.", m_GestureId.c_str(), entry.m_AnimId.c_str() );
		behavior.async<void>( "runBehavior", entry.m_AnimId ).wait();
	}
	else
	{
		Log::Warning( "NaoAnimateGesture", "No valid animations found for gesture %s", m_GestureId.c_str() );
	}
}

void NaoAnimateGesture::AnimateDone( Request * a_pReq )
{
	// pop the request, if true is returned then start the next request..
	if ( PopRequest() )
		ThreadPool::Instance()->InvokeOnThread(DELEGATE(NaoAnimateGesture, AnimateThread, Request * , this), ActiveRequest());
}
