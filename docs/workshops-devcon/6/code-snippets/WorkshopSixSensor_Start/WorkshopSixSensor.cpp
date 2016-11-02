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

#include "WorkshopSixSensor.h"

REG_SERIALIZABLE(WorkshopSixSensor);
RTTI_IMPL(WorkshopSixSensor, Camera);

#ifndef _WIN32
REG_OVERRIDE_SERIALIZABLE(Camera, WorkshopSixSensor);
#endif

//  OnStart() will create and subscribe a camera object to the Blackboard on a background thread.
bool WorkshopSixSensor::OnStart()
{
    Log::Debug("WorkshopSixSensor", "Starting Camera!");
    m_StopThread = false;
    m_ThreadStopped = false;
    return true;
}

//  OnStop() when called will kill any camera thread that is running ensuring that the process is stopped.
bool WorkshopSixSensor::OnStop()
{
    Log::Debug("WorkshopSixSensor", "Stopping MacCamera!");
    m_StopThread = true;
    while (!m_ThreadStopped)
        tthread::this_thread::yield();
    return true;
}

//  CaptureVideo() first checks that the thread is still live. If the thread is, then the frame object is then encoded
//  into the memory buffer as a JPEG. This then sends the buffered data.
void WorkshopSixSensor::CaptureVideo(void * arg)
{

}

//  OnPaused() will increment the variable Paused, each time it is called.
void WorkshopSixSensor::OnPause()
{
    m_Paused++;
}

//  OnResume() Will decrease the variable Paused each time it is called.
void WorkshopSixSensor::OnResume()
{
    m_Paused--;
}

//  SendingData will check to see if the frame has been paused. This function should call only call the SendData()
//  If variable m_Paused is greater than zero.
void WorkshopSixSensor::SendingData(VideoData * a_pData)
{

}