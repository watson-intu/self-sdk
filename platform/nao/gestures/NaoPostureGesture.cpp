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
#include "NaoPostureGesture.h"
#include "SelfInstance.h"
#include "utils/ThreadPool.h"

REG_OVERRIDE_SERIALIZABLE( PostureGesture, NaoPostureGesture );
RTTI_IMPL( NaoPostureGesture, PostureGesture );

void NaoPostureGesture::Serialize(Json::Value & json)
{
	PostureGesture::Serialize(json);
	json["m_bStiffnessControl"] = m_bStiffnessControl;
}

void NaoPostureGesture::Deserialize(const Json::Value & json)
{
	PostureGesture::Deserialize(json);
	if ( json.isMember("m_bStiffnessControl") )
		m_bStiffnessControl = json["m_bStiffnessControl"].asBool();
}

bool NaoPostureGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	if ( PushRequest( a_Callback, a_Params ) )
		ThreadPool::Instance()->InvokeOnThread(  DELEGATE( NaoPostureGesture, AnimateThread, Request *, this ), ActiveRequest() );
	return true;
}

bool NaoPostureGesture::Abort()
{
	return false;
}

void NaoPostureGesture::AnimateThread( Request * a_pReq )
{
	try {
		DoAnimateThread(a_pReq);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoPostureGesture", "Caught Exception: %s", ex.what() );
		a_pReq->m_bError = true;
	}

	//// now invoke the main thread to notify them the move is completed..
	ThreadPool::Instance()->InvokeOnMain( DELEGATE(NaoPostureGesture, AnimateDone, Request *, this ), a_pReq );
}

void NaoPostureGesture::DoAnimateThread(Request *a_pReq)
{
	NaoPlatform * pPlatform = NaoPlatform::Instance();

	qi::AnyObject motion = pPlatform->GetSession()->service( "ALMotion" );
	if ( m_PostureId == "wakeUp" )
	{
		motion.call<void>( "wakeUp" );
		pPlatform->SetPostureId( "Stand" );
	}
	else if ( m_PostureId == "rest" )
	{
		motion.call<void>( "rest" );
		pPlatform->SetPostureId( "Crouch" );
	}
	else
	{
		if ( m_PostureId == "LyingBack" )
		{
			Log::Debug( "NaoPlatform", "Turning off autonomous life in lying posture." );
			qi::AnyObject life = pPlatform->GetSession()->service("ALAutonomousLife");
			life.call<void>("setState", "disabled");
		}
		bool bEnableFallManager = !m_bDisableFallManager;
		motion.async<void>( "setFallManagerEnabled", bEnableFallManager ).wait();

		qi::Future<bool> action = pPlatform->SetPosture(m_PostureId, m_fSpeed);
		if (!action.value())
			Log::Error("NaoPostureGesture", "Failed to set posture to %s.", m_PostureId.c_str());
	}
}

void NaoPostureGesture::AnimateDone( Request * a_pReq )
{
	// pop the request, if true is returned then start the next request..
	if ( PopRequest() )
		ThreadPool::Instance()->InvokeOnThread( DELEGATE( NaoPostureGesture, AnimateThread, Request *, this ), ActiveRequest() );
}
