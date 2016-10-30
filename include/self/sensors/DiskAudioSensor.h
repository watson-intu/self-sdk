/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#ifndef SELF_DISKAUDIOSENSOR_H
#define SELF_DISKAUDIOSENSOR_H

#include "sensors/Microphone.h"

class DiskAudioSensor : public Microphone
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<DiskAudioSensor>		SP;
	typedef boost::weak_ptr<DiskAudioSensor>		WP;

	//! Construction
	DiskAudioSensor();
	~DiskAudioSensor();

	//! ISerializable interface
	void Serialize(Json::Value &json);
	void Deserialize(const Json::Value &json);

	//! ISensor interface
	virtual const char *GetSensorName() 
	{
		return "Microphone";
	}
	virtual const char * GetDataType()
	{
		return AudioData::GetStaticRTTI().GetName().c_str();
	}

	const std::string & GetRecordingsPath() const
	{
		return m_RecordingsPath;
	}


	bool GetCaptureStopped()
	{
		return m_CaptureStopped;
	}

	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

private:
	//! Data
	volatile bool m_CaptureAudio;
	volatile bool m_Paused;
	volatile bool m_CaptureStopped;

	std::string m_RecordingsPath;
	bool m_Error;
	void *m_CaptureEvents[2];

	void CaptureAudio(void *);
	void SendAudio(AudioData *a_pData);
};

#endif //SELF_DISKAUDIOSENSOR_H
