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

#include "Nao3DCamera.h"
#include "SelfInstance.h"

#ifndef _WIN32
#include <qi/os.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <alvision/alvisiondefinitions.h>
#include <alvision/alimage.h>
#include <alproxies/alvideodeviceproxy.h>
#endif

#include "tinythread++/tinythread.h"

#ifndef _WIN32
REG_SERIALIZABLE(Nao3DCamera);
#endif
RTTI_IMPL(Nao3DCamera, DepthCamera);

bool Nao3DCamera::OnStart()
{
    Log::Debug("Nao3DCamera", "Starting up video device");

    m_StopThread = false;
    ThreadPool::Instance()->InvokeOnThread<void *>( DELEGATE(Nao3DCamera, StreamingThread, void *, this ), NULL );
    return true;
}

bool Nao3DCamera::OnStop()
{
    m_StopThread = true;
    while(! m_ThreadStopped )
        tthread::this_thread::yield();
    return true;
}

void Nao3DCamera::StreamingThread(void * arg)
{
    try
    {
        DoStreamingThread(arg);
    }
    catch( const std::exception & ex )
    {
        Log::Error( "Nao3DCamera", "Caught Exception: %s", ex.what() );
    }
    m_ThreadStopped = true;
}

void Nao3DCamera::DoStreamingThread(void *arg)
{
#ifndef _WIN32
//    const std::string & robotIp = SelfInstance::GetInstance()->GetRobotIp();
    AL::ALVideoDeviceProxy  camProxy("127.0.0.1", 9559);
    camProxy.setParam(AL::kCameraSelectID, 2);
    m_ClientName = camProxy.subscribe("test", AL::kQVGA, 17, 15);

    AL::ALValue lImage;
    lImage.arraySetSize(7);

    while(!m_StopThread)
    {
        cv::Mat imgHeader = cv::Mat(cv::Size(320, 240), CV_16UC1);

        AL::ALValue img = camProxy.getImageRemote(m_ClientName);
        if(img.getSize() != 12) {
            Log::Error("Nao3DCamera", "Image Size: %d", img.getSize());
            tthread::this_thread::sleep_for(tthread::chrono::milliseconds(3000));
            continue;
        }
        imgHeader.data = (uchar*)img[6].GetBinary();


//        cv::imwrite("test_17.png", imgHeader * 8);

        std::vector<unsigned char> outputVector;
        if ( cv::imencode(".png", imgHeader * 8, outputVector) && m_Paused <= 0 )
            ThreadPool::Instance()->InvokeOnMain<DepthVideoData *>( DELEGATE( Nao3DCamera, SendingData, DepthVideoData *, this ), new DepthVideoData(outputVector));
        else
            Log::Error( "Nao3DCamera", "Failed to imencode()" );

        camProxy.releaseImage(m_ClientName);

        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(3000));
    }

    Log::Debug("Nao3DCamera", "Closing Video feed with m_ClientName: %s", m_ClientName.c_str());
    camProxy.unsubscribe(m_ClientName);
    Log::Status("Nao3DCamera", "Stopped video device");
#endif
}

void Nao3DCamera::SendingData( DepthVideoData * a_pData )
{
    SendData( a_pData );
}

void Nao3DCamera::OnPause()
{
    m_Paused++;
}

void Nao3DCamera::OnResume()
{
    m_Paused--;
}


