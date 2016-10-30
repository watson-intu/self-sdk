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

#include "LinuxCamera.h"
#include "SelfInstance.h"

#ifndef _WIN32
#include <qi/os.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif

#ifndef _WIN32
REG_SERIALIZABLE(LinuxCamera);
REG_OVERRIDE_SERIALIZABLE(Camera, LinuxCamera);
#endif

RTTI_IMPL(LinuxCamera, Camera);

bool LinuxCamera::OnStart()
{
 	Log::Debug("LinuxCamera", "Starting up video device");

	m_StopThread = false;
	ThreadPool::Instance()->InvokeOnThread<void *>( DELEGATE(LinuxCamera, StreamingThread, void *, this ), NULL );
	return true;
}

bool LinuxCamera::OnStop()
{
	m_StopThread = true;
	while(! m_ThreadStopped )
		tthread::this_thread::yield();
	return true;
}

void LinuxCamera::StreamingThread(void * arg)
{
	try
	{
		DoStreamingThread(arg);
	}
	catch( const std::exception & ex )
	{
		Log::Error( "LinuxCamera", "Caught Exception: %s", ex.what() );
	}
	;
	m_ThreadStopped = true;
}

void LinuxCamera::DoStreamingThread(void *arg)
{ 
#ifndef _WIN32
  CvCapture* capture = 0;
  cv:: Mat cameraFrame, frameCopy, image;
  
  capture = cvCaptureFromCAM( 0 );
  
  while(!m_StopThread)
    {  
      IplImage* iplImg = cvQueryFrame( capture );
      cameraFrame = iplImg;
      /* 
	 vector<int> compression_params;
	 compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	 compression_params.push_back(9);
	 imwrite("/home/easisbot/Desktop/test.png", cameraFrame, compression_params); 
      */

      std::vector<int> p;
      p.push_back(3);
      p.push_back(10);
      
      std::vector<unsigned char> outputVector;
      if ( cv::imencode(".jpg", cameraFrame, outputVector, p) && m_Paused <= 0 )
	ThreadPool::Instance()->InvokeOnMain<VideoData *>( DELEGATE( LinuxCamera, SendingData, VideoData *, this ), new VideoData(outputVector));
      else
	Log::Error( "LinuxCamera", "Failed to imencode()" );
      
      boost::this_thread::sleep_for(boost::chrono::milliseconds(3000));
    }
  
  Log::Debug("LinuxCamera", "Closing Video feed with m_ClientName: %s", m_ClientName.c_str());
#endif

  Log::Status("LinuxCamera", "Stopped video device");

  m_ThreadStopped = true;
}

void LinuxCamera::SendingData( VideoData * a_pData )
{
	SendData( a_pData );
}

void LinuxCamera::OnPause()
{
	m_Paused++;
}

void LinuxCamera::OnResume()
{
	m_Paused--;
}


