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

#ifndef SELF_MICROPHONE_H
#define SELF_MICROPHONE_H

#include <list>

#include "AudioData.h"
#include "sensors/ISensor.h"
#include "SelfLib.h"

//! This is the base class for a standard microphone, this is not a concrete implementation. For that
//! see DSMicrophone or NaoMicrophone
class SELF_API Microphone : public ISensor
{
public:
	RTTI_DECL();

	//! Construction
	Microphone() : m_RecordingHZ( 16000 ),
		m_RecordingBits( 16 ), 
		m_RecordingChannels( 1 ),
		m_BinaryType( "audio/L16;rate=16000;channels=1" )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "Microphone";
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
};

#endif
