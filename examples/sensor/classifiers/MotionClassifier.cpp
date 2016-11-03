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

#include "MotionClassifier.h"

REG_SERIALIZABLE(MotionClassifier);
RTTI_IMPL( MotionClassifier, IClassifier);

void MotionClassifier::Serialize(Json::Value & json)
{
    IClassifier::Serialize( json );
}

void MotionClassifier::Deserialize(const Json::Value & json)
{
    IClassifier::Deserialize( json );
}

const char * MotionClassifier::GetName() const
{
    return "MotionClassifier";
}

bool MotionClassifier::OnStart()
{
    BlackBoard * pBlackboard = SelfInstance::GetInstance()->GetBlackBoard();
    //TE_Added just give me all the things being thrown to the blackboard
    pBlackboard->SubscribeToType( Motion::GetStaticRTTI(),
                                  DELEGATE( MotionClassifier, OnMotion, const ThingEvent &, this ), TE_ADDED );


    Log::Status("MotionClassifier", "MotionClassifier started");
    return true;
}

bool MotionClassifier::OnStop()
{
    Log::Status("MotionClassifier", "Motion Classifier stopped");
    BlackBoard * pBlackboard = SelfInstance::GetInstance()->GetBlackBoard();
    pBlackboard->UnsubscribeFromType( Motion::GetStaticRTTI(), this );
    return true;
}

void MotionClassifier::OnMotion(const ThingEvent & a_ThingEvent)
{
    Motion::SP spMotion = DynamicCast<Motion>(a_ThingEvent.GetIThing());
    if(spMotion)
    {
        Log::Debug("MotionClassifier", "Classifying Motion");
//        if ( spMotion->IsActive() )
//        {
//			Log::Debug("MotionClassifier", "Changing State!");
//            spMotion->SetState("active");
//        }
    }
}

