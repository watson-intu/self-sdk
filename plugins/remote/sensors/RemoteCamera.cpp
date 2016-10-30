//
// Created by John Andersen on 6/23/16.
//

#include "RemoteCamera.h"
#include "SelfInstance.h"

REG_OVERRIDE_SERIALIZABLE( Camera, RemoteCamera );
REG_SERIALIZABLE(RemoteCamera);
RTTI_IMPL(RemoteCamera, ISensor);

bool RemoteCamera::OnStart()
{
	SelfInstance * pInstance = SelfInstance::GetInstance();

	ITopics * pTopics = pInstance->GetTopics();
	pTopics->RegisterTopic( "video-input", "image/jpeg" );
	pTopics->Subscribe( "video-input", DELEGATE( RemoteCamera, OnRemoteVideo, const ITopics::Payload &, this ) );

	m_pCameraService = pInstance->FindService<PTZCamera>();
	if ( m_pCameraService != NULL )
		m_spWaitTimer = TimerPool::Instance()->StartTimer(VOID_DELEGATE(RemoteCamera, OnRequestImage, this), 1.0f / m_fFramesPerSec, true, true);

    return true;
}

bool RemoteCamera::OnStop()
{
    m_spWaitTimer.reset();
    return true;
}

void RemoteCamera::OnPause()
{
    m_Paused++;
}

void RemoteCamera::OnResume()
{
    m_Paused--;
}

void RemoteCamera::OnRequestImage()
{
	if (! m_bProcessing && m_Paused <= 0 )
	{
		m_bProcessing = true;
		m_pCameraService->GetImage(DELEGATE(RemoteCamera, OnGetImage, const std::string & , this));
	}
}

void RemoteCamera::OnGetImage( const std::string & a_Image )
{
	ThreadPool::Instance()->InvokeOnMain<VideoData *>( DELEGATE( RemoteCamera, OnSendData, VideoData *, this ), 
		new VideoData((const unsigned char *)a_Image.c_str(), a_Image.size()));
}

void RemoteCamera::OnRemoteVideo( const ITopics::Payload & a_Payload )
{
	OnSendData( new VideoData( (unsigned char *)a_Payload.m_Data.c_str(), a_Payload.m_Data.size() ) );
}

void RemoteCamera::OnSendData( VideoData * a_pData )
{
	SendData( a_pData );
	m_bProcessing = false;
}

