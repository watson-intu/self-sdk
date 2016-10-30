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

#include "NaoCamera.h"
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
REG_SERIALIZABLE(NaoCamera);
REG_OVERRIDE_SERIALIZABLE( Camera, NaoCamera);
#endif

RTTI_IMPL(NaoCamera, Camera);

bool NaoCamera::OnStart()
{
	Log::Debug("NaoVideo", "Starting up video device");

	m_StopThread = false;
	ThreadPool::Instance()->InvokeOnThread<void *>( DELEGATE(NaoCamera, StreamingThread, void *, this ), NULL );
	return true;
}

bool NaoCamera::OnStop()
{
	m_StopThread = true;
	while(! m_ThreadStopped )
		tthread::this_thread::yield();
	return true;
}

void NaoCamera::StreamingThread(void * arg)
{
	try
	{
		DoStreamingThread(arg);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "NaoCamera", "Caught Exception: %s", ex.what() );
	}
	m_ThreadStopped = true;
}

void NaoCamera::DoStreamingThread(void *arg)
{
#ifndef _WIN32
	std::string robotIp("127.0.0.1");
	SelfInstance * pInstance = SelfInstance::GetInstance();
	if ( pInstance != NULL )
		robotIp = pInstance->GetRobotIp();

	AL::ALVideoDeviceProxy  camProxy(robotIp.c_str(), 9559);
	//Use the below instantiation if you want HIGHEST quality photos
	//clientName = camProxy.subscribe("test", AL::k4VGA, AL::kBGRColorSpace, 30);
	m_ClientName = camProxy.subscribe("test", AL::kQVGA, AL::kBGRColorSpace, 30);

	AL::ALValue lImage;
	lImage.arraySetSize(7);

	while(!m_StopThread)
	{
		cv::Mat imgHeader = cv::Mat(cv::Size(320, 240), CV_8UC3);

		AL::ALValue img = camProxy.getImageRemote(m_ClientName);
		if(img.getSize() != 12) {
			Log::Error("NaoCamera", "Image Size: %d", img.getSize());
			tthread::this_thread::sleep_for(tthread::chrono::milliseconds(3000));
			continue;
		}
		imgHeader.data = (uchar*)img[6].GetBinary();

		std::vector<int> p;
		p.push_back(3);
		p.push_back(10);

		std::vector<unsigned char> outputVector;
		if ( cv::imencode(".jpg", imgHeader, outputVector, p) && m_Paused <= 0 )
			ThreadPool::Instance()->InvokeOnMain<VideoData *>( DELEGATE( NaoCamera, SendingData, VideoData *, this ), new VideoData(outputVector));
		else
			Log::Error( "NaoCamera", "Failed to imencode()" );

		camProxy.releaseImage(m_ClientName);

		tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1000 / m_fFramesPerSec));
	}

	Log::Debug("NaoVideo", "Closing Video feed with m_ClientName: %s", m_ClientName.c_str());
	camProxy.unsubscribe(m_ClientName);
	Log::Status("NaoVideo", "Stopped video device");
#endif
}

void NaoCamera::SendingData( VideoData * a_pData )
{
	SendData( a_pData );
}

void NaoCamera::OnPause()
{
	m_Paused++;
}

void NaoCamera::OnResume()
{
	m_Paused--;
}


