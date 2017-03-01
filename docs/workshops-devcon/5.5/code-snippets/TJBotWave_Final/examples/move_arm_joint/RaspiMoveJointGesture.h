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

#ifndef SELF_MoveArmJointGesture_H
#define SELF_MoveArmJointGesture_H

#include "gestures/MoveJointGesture.h"

//! This is the class for waving arm joints for RaspberryPI
class RaspiMoveJointGesture : public MoveJointGesture
{
public:
	RTTI_DECL();
	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();
	//! Construction
	RaspiMoveJointGesture() : m_PinNumber( 7 ), m_bWiredPi( false ), m_ArmLowPoint(0), m_ArmHighPoint(40)
	{}
private:
	//! Data
	bool m_bWiredPi;
	int m_PinNumber;
	int m_ArmLowPoint;
	int m_ArmHighPoint;
	static int m_CurrentPWMArmState;

	//! Callbacks
	void MoveJointThread( Request * a_pReq );
	void MoveJointDone( Request * a_pReq );
	void MoveArm();
	int PWMValue();
};
#endif
