//
// Created by John Andersen on 6/23/16.
//

#ifndef SELF_REMOTECAMERA_H
#define SELF_REMOTECAMERA_H

#include "sensors/Camera.h"
#include "utils/TimerPool.h"
#include "services/PTZCamera.h"
#include "sensors/VideoData.h"

class RemoteCamera : public Camera
{
public:
    RTTI_DECL();

    //! Construction
    RemoteCamera() : m_pCameraService( NULL ), m_Paused( 0 ), m_bProcessing(false)
    {}

    //! ISensor interface
    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();


private:
    //!Data
	PTZCamera *				m_pCameraService;
    TimerPool::ITimer::SP   m_spWaitTimer;
    volatile int			m_Paused;
	bool					m_bProcessing;

	//! Callbacks
    void                    OnRequestImage();
	void                    OnGetImage(const std::string & a_Image);
	void					OnRemoteVideo( const ITopics::Payload & a_Payload );
	void					OnSendData( VideoData * a_pData );
};

#endif //SELF_REMOTECAMERA_H
