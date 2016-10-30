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

#ifndef NAO_POSTURE_GESTURE_H
#define NAO_POSTURE_GESTURE_H

#include "gestures/PostureGesture.h"

//! This is the base class for any gesture that moves a single joint on a robot.
class NaoPostureGesture : public PostureGesture
{
public:
	RTTI_DECL();

	//! Construction
	NaoPostureGesture() : m_bStiffnessControl( false )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

private:
	//! Callbacks
	void AnimateThread( Request * a_pReq );
	void DoAnimateThread( Request * a_pReq );
	void AnimateDone( Request * a_pReq );

	//! Data
	bool m_bStiffnessControl; // to indicate whether the PostureId falls under the Stiffness Control API
};


#endif // NAO_ANIMATE_GESTURE_H
