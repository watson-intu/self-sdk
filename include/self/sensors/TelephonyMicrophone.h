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

#ifndef SELF_TELE_MICROPHONE_H
#define SELF_TELE_MICROPHONE_H

#include "AudioData.h"
#include "Microphone.h"
#include "SelfLib.h"

class SELF_API TelephonyMicrophone : public Microphone
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<TelephonyMicrophone>		SP;
	typedef boost::weak_ptr<TelephonyMicrophone>		WP;

	//! Construction
	TelephonyMicrophone() : m_RecordingHZ( 16000 ),
	               m_RecordingBits( 16 ),
	               m_RecordingChannels( 1 ),
	               m_BinaryType( "audio/L16;rate=16000;channels=1" )
	{}

	void SendAudioData( const std::string & a_Data );

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);
	
	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "TelephonyMicrophone";
	}
	virtual const char * GetDataType()
	{
		return AudioData::GetStaticRTTI().GetName().c_str();
	}
	virtual const char * GetBinaryType()
	{
		return m_BinaryType.c_str();
	}
	
	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

protected:
	//! Data
	int m_RecordingHZ;
	int m_RecordingBits;
	int m_RecordingChannels;
	std::string m_BinaryType;

private:
	void				SendingData( AudioData * a_pData );
};

#endif
