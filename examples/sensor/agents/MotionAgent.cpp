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

#include "MotionAgent.h"
#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/IThing.h"
#include "blackboard/Say.h"

REG_SERIALIZABLE(MotionAgent);
RTTI_IMPL( MotionAgent, IAgent);

void MotionAgent::Serialize(Json::Value & json)
{
    IAgent::Serialize(json);
 
}

void MotionAgent::Deserialize(const Json::Value & json)
{
    IAgent::Deserialize(json);
 
}

bool MotionAgent::OnStart()
{
    SelfInstance::GetInstance()->GetBlackBoard()->SubscribeToType(Motion::GetStaticRTTI(),
                                                                  DELEGATE( MotionAgent, OnMotion, const ThingEvent &, this ), TE_ADDED);
    return true;
}

bool MotionAgent::OnStop()
{
    

    SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType(Motion::GetStaticRTTI(), this);
    //SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType(NameIntent::GetStaticRTTI(), this);

    return true;
}



void MotionAgent::OnMotion(const ThingEvent & a_ThingEvent)
{
    Motion::SP spMotion = DynamicCast<Motion>(a_ThingEvent.GetIThing());
	Log::Debug("MotionAgent", "RECEIVED MOTION EVENT!!");
	if (spMotion)
	{
		Log::Debug("MotionAgent", "Status of motion is %d", spMotion->m_Active);
		if (true) {
			Log::Debug("MotionAgent", "MOTION IS ACTIVE!");
			SelfInstance::GetInstance()->GetBlackBoard()->AddThing(
				Say::SP(new Say("Motion has been detected")));
		}
	}
}






