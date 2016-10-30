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
#include "NaoMoveJointGesture.h"
#include "utils/ThreadPool.h"
#include "SelfInstance.h"

#pragma warning(disable:4275)
#include "qi/session.hpp"

REG_OVERRIDE_SERIALIZABLE(MoveJointGesture, NaoMoveJointGesture);
RTTI_IMPL( NaoMoveJointGesture, MoveJointGesture );

bool NaoMoveJointGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
	if ( PushRequest( a_Callback, a_Params ) )
		ThreadPool::Instance()->InvokeOnThread(VOID_DELEGATE(NaoMoveJointGesture, MoveThread, this));
	return true;
}

void NaoMoveJointGesture::MoveThread()
{
	try {
		DoMoveThread();
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoMoveJointGesture", "Caught Exception: %s", ex.what() );
		ActiveRequest()->m_bError = true;
	}

	ThreadPool::Instance()->InvokeOnMain(VOID_DELEGATE(NaoMoveJointGesture, MoveDone, this) );
}

void NaoMoveJointGesture::DoMoveThread()
{
	Request * pRequest = ActiveRequest();
	assert( pRequest != NULL );

	NaoPlatform::Instance()->DisableExtraMovement();

	qi::AnyObject motion = NaoPlatform::Instance()->GetSession()->service("ALMotion");

	std::list< qi::Future<void> > motions;
	for(size_t i=0;i<m_JointNames.size();++i)
	{
		Log::Debug("NaoMoveJointGesture", "Moving joint %s to angle: %f, speed: %f, stiffness: %f, absolute: %s",
				   m_JointNames[i].c_str(), m_fAngles[i], m_fSpeeds[i], m_fStiffness, m_bAbsolute ? "Yes" : "No");

		motion.async<void>("setStiffnesses", m_JointNames[i], m_fStiffness).wait();
		motions.push_back( motion.async<void>("angleInterpolation", m_JointNames[i], m_fAngles[i], m_fSpeeds[i], m_bAbsolute) );
	}

	//! wait for all motions to complete
	while( motions.begin() != motions.end() )
	{
		motions.front().wait();
		motions.pop_front();
	}

	for(size_t i=0;i<m_JointNames.size();++i)
		motion.async<void>("setStiffnesses", m_JointNames[i], 0.0f).wait();
}

void NaoMoveJointGesture::MoveDone()
{
	if ( PopRequest() )
		ThreadPool::Instance()->InvokeOnThread(VOID_DELEGATE(NaoMoveJointGesture, MoveThread, this));
}
