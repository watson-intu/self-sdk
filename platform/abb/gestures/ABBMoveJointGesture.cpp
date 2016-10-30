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

#include "ABBPlatform.h"
#include "ABBMoveJointGesture.h"
#include "utils/ThreadPool.h"
#include "SelfInstance.h"

#include <boost/thread.hpp>

REG_OVERRIDE_SERIALIZABLE(MoveJointGesture, ABBMoveJointGesture);
RTTI_IMPL(ABBMoveJointGesture, MoveJointGesture);

bool ABBMoveJointGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
	if (PushRequest(a_Callback, a_Params))
		ThreadPool::Instance()->InvokeOnThread(VOID_DELEGATE(ABBMoveJointGesture, MoveThread, this));
	return true;
}

void ABBMoveJointGesture::MoveThread()
{
	try
	{
		DoMoveThread();
	}
	catch (const std::exception & ex)
	{
		Log::Error("ABBMoveJointGesture", "Caught Exception: %s", ex.what());
		ActiveRequest()->m_bError = true;
	}

	// now invoke the main thread to notify them the move is completed..
	ThreadPool::Instance()->InvokeOnMain(VOID_DELEGATE(ABBMoveJointGesture, MoveDone, this));
}

void ABBMoveJointGesture::DoMoveThread()
{
	m_GoalId = UniqueID().Get();
	m_GoalDone = false;

	for(size_t i=0;i<m_JointNames.size();++i)
		ABBPlatform::Instance()->Send(m_JointNames[i], m_fAngles[i] );
}

void ABBMoveJointGesture::MoveDone()
{
	if (PopRequest())
		ThreadPool::Instance()->InvokeOnThread(VOID_DELEGATE(ABBMoveJointGesture, MoveThread, this));
}
