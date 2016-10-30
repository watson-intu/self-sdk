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

#include "RosPlatform.h"
#include "RosMoveJointGesture.h"
#include "utils/ThreadPool.h"
#include "SelfInstance.h"

#include <boost/thread.hpp>

REG_OVERRIDE_SERIALIZABLE(MoveJointGesture, RosMoveJointGesture);
RTTI_IMPL(RosMoveJointGesture, MoveJointGesture);

const float TIMEOUT = 30.0f;

bool RosMoveJointGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
    if ( PushRequest( a_Callback, a_Params ) )
        ThreadPool::Instance()->InvokeOnThread(VOID_DELEGATE(RosMoveJointGesture, MoveThread, this));
    return true;
}

void RosMoveJointGesture::MoveThread()
{
    try {
        DoMoveThread();
    }
    catch( const std::exception & ex )
    {
        Log::Error( "RosMoveJointGesture", "Caught Exception: %s", ex.what() );
		ActiveRequest()->m_bError = true;
    }

	// now invoke the main thread to notify them the move is completed..
	ThreadPool::Instance()->InvokeOnMain(VOID_DELEGATE(RosMoveJointGesture, MoveDone, this));
}

void RosMoveJointGesture::DoMoveThread()
{
	RosPlatform::Instance()->Subscribe("/goal_status", "actionlib_msgs/GoalStatusArray",
		DELEGATE(RosMoveJointGesture, OnGoalStatus, const Json::Value &, this));

	m_GoalId = UniqueID().Get();
	m_GoalDone = false;

    Json::Value json;
    json["originator"] = "c5console";
    json["header"]["seq"] = 0;
    json["header"]["frame_id"] = m_GoalId;
    json["header"]["stamp"]["secs"] = 0;
    json["header"]["stamp"]["nsecs"] = 0;
	for (size_t i = 0; i < m_JointNames.size(); i++)
	{
		json["joint_names"][i] = m_JointNames[i];
		json["points"][0]["positions"][i] = m_fAngles[i];
		json["points"][0]["velocities"][i] = 0;
		json["points"][0]["accelerations"][i] = 0;
	}
    json["points"][0]["time_from_start"]["secs"] = 0;
    json["points"][0]["time_from_start"]["nsecs"] = 0;
    Log::Debug("RosMoveJointGesture", "Sending Command: %s", Json::FastWriter().write( json ).c_str() );
    RosPlatform::Instance()->Publish("/joint_refs", "r2_msgs/LabeledJointTrajectory", json); 

	m_spWaitTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(RosMoveJointGesture, OnTimeout, this), TIMEOUT, true, false);
	while (!m_GoalDone) 
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	m_spWaitTimer.reset();

	RosPlatform::Instance()->Unsubscribe("/goal_status", this );
}

void RosMoveJointGesture::OnGoalStatus(const Json::Value & a_response)
{
	//Log::Status("RosMoveJointGesture", "Goal Status: %s", a_response.toStyledString().c_str());
	if (a_response.isMember("msg"))
	{
		const Json::Value & msg = a_response["msg"];
		if (msg.isMember("status_list"))
		{
			const Json::Value & status_list = msg["status_list"];
			for (size_t i = 0; i < status_list.size(); ++i)
			{
				const Json::Value & status = status_list[i];
				if (!status.isMember("goal_id"))
					continue;
				if (status["goal_id"]["id"].asString() != m_GoalId)
					continue;
				int result = status["status"].asInt();
				Log::Status("RosMoveJointGresture", "Joint %s status %d", m_GestureId.c_str(), result);
				if (result >= 3)
					m_GoalDone = true;
			}
		}
	}
}

void RosMoveJointGesture::MoveDone()
{
    if ( PopRequest() )
        ThreadPool::Instance()->InvokeOnThread(VOID_DELEGATE(RosMoveJointGesture, MoveThread, this));
}

void RosMoveJointGesture::OnTimeout()
{
	m_GoalDone = true;
}
