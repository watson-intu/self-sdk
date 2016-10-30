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

#include "gestures/GraspGesture.h"
#include "qi/session.hpp"

#ifndef SELF_NAOGRASPGESTURE_H
#define SELF_NAOGRASPGESTURE_H

//! This is the base class for any gesture that involve moving hands on a robot.
class NaoGraspGesture : public GraspGesture
{
public:
    RTTI_DECL();

    //! IGesture interface
    virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
    virtual bool Abort();

private:
    GestureDelegate		m_Callback;
    ParamsMap			m_Params;
    qi::Future<void>	m_MoveOp;

    //! Callbacks
    void MoveThread();
    void DoMoveThread();
    void MoveDone();
    bool DoAbort();
};


#endif //SELF_NAOGRASPGESTURE_H
