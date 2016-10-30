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

#include "SelfInstance.h"
#include "NaoPlatform.h"
#include "NaoVolumeGesture.h"
#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Health.h"
#include "utils/ThreadPool.h"

REG_OVERRIDE_SERIALIZABLE( VolumeGesture, NaoVolumeGesture );
RTTI_IMPL( NaoVolumeGesture, VolumeGesture );

bool NaoVolumeGesture::Execute( GestureDelegate a_Callback, const ParamsMap & a_Params )
{
	if ( PushRequest( a_Callback, a_Params ) )
		ThreadPool::Instance()->InvokeOnThread(  DELEGATE( NaoVolumeGesture, VolumeThread, Request *, this ), ActiveRequest() );
	return true;
}

void NaoVolumeGesture::VolumeThread( Request * a_pReq )
{
	try {
		DoVolumeThread(a_pReq);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoVolumeGesture", "Caught Exception: %s", ex.what() );
	}

	// now invoke the main thread to notify them the move is completed..
	ThreadPool::Instance()->InvokeOnMain( DELEGATE(NaoVolumeGesture, VolumeDone, Request *, this ), a_pReq );
}

void NaoVolumeGesture::DoVolumeThread( Request * a_pReq )
{
	qi::AnyObject audioDevice = NaoPlatform::Instance()->GetSession()->service("ALAudioDevice");

	if ( a_pReq != NULL )
	{
		if ( a_pReq->m_Params.GetData().isMember("m_fChange") )
			m_fChange = a_pReq->m_Params["m_fChange"].asFloat();
		if ( a_pReq->m_Params.GetData().isMember("m_fTargetVolume") )
			m_fTargetVolume = a_pReq->m_Params["m_fTargetVolume"].asFloat();
	}

	if ( m_fChange != 0 )
	{
		int currentVolume = audioDevice.call<int>( "getOutputVolume" );
		m_fTargetVolume = currentVolume + m_fChange;
	}

	if ( m_fTargetVolume < 0 )
		m_fTargetVolume = 0;
	if ( m_fTargetVolume > 100 )
		m_fTargetVolume = 100;

	audioDevice.call<void>( "setOutputVolume", m_fTargetVolume );
}

void NaoVolumeGesture::VolumeDone( Request * a_pReq )
{
	// DISABLED: Gestures should not be modifying the blackboard!
	//SelfInstance * pInstance = SelfInstance::GetInstance();
	//if ( pInstance != NULL )
	//	pInstance->GetBlackBoard()->AddThing(Health::SP(new Health("volume", "", m_fTargetVolume, false)));

	// pop the request, if true is returned then start the next request..
	if ( PopRequest() )
		ThreadPool::Instance()->InvokeOnThread( DELEGATE( NaoVolumeGesture, VolumeThread, Request *, this ), ActiveRequest() );
}

bool NaoVolumeGesture::Abort()
{
	return false;
}
