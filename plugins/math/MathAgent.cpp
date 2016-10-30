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

#include "MathAgent.h"
#include "MathIntent.h"
#include "SelfInstance.h"
#include "blackboard/BlackBoard.h"
#include "blackboard/Say.h"

REG_SERIALIZABLE(MathAgent);
RTTI_IMPL( MathAgent, IAgent);


void MathAgent::Serialize(Json::Value & json)
{
	IAgent::Serialize(json);
}

void MathAgent::Deserialize(const Json::Value & json)
{
	IAgent::Deserialize(json);
}

bool MathAgent::OnStart()
{
	SelfInstance::GetInstance()->GetBlackBoard()->SubscribeToType(MathIntent::GetStaticRTTI().GetName(),
		DELEGATE( MathAgent, OnMathOperation, const ThingEvent &, this ), TE_ADDED );
	return true;
}

bool MathAgent::OnStop()
{
	SelfInstance::GetInstance()->GetBlackBoard()->UnsubscribeFromType(MathIntent::GetStaticRTTI().GetName(), this);
	return true;
}

void MathAgent::OnMathOperation(const ThingEvent & a_ThingEvent)
{
	MathIntent::SP spOp = DynamicCast<MathIntent>( a_ThingEvent.GetIThing() );
	if( spOp )
	{
		spOp->AddChild( Say::SP( new Say( "I can do math someday!" ) ) );
	}
}

