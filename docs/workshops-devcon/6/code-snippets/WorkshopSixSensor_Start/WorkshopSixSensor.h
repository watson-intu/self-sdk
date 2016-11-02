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

#ifndef SELF_WORKSHOP_SIX_SENSOR_H
#define SELF_WORKSHOP_SIX_SENSOR_H

#include "sensors/Camera.h"
#include "SelfInstance.h"
#include "utils/TimerPool.h"

#ifndef _WIN32
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#endif

//! Workshop Six Sensor
class WorkshopSixSensor : public Camera
{
public:
    RTTI_DECL();

    //! Construction
    WorkshopSixSensor() : m_StopThread(false), m_ThreadStopped(false), m_Paused(0)
    {}

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "Camera";
    }

    //! ISensor interface
    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

    void				SendingData(VideoData * a_pData);

private:

    //! Data
    volatile bool		m_StopThread;
    volatile bool		m_ThreadStopped;
    volatile int 		m_Paused;
#ifndef _WIN32
    cv::VideoCapture    m_Capture;
#endif

    void                CaptureVideo(void * arg);
};

#endif //SELF_WORKSHOP_SIX_SENSOR_H
