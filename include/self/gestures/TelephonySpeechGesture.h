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

#ifndef TELE_SPEECH_GESTURE_H
#define TELE_SPEECH_GESTURE_H

#include "gestures/SpeechGesture.h"

class Sound;
class TextToSpeech;
class Telephony;
struct Voices;

//! This gesture wraps the local speech sythesis so the self can speak
class SELF_API TelephonySpeechGesture : public SpeechGesture
{
public:

//! Types
typedef std::vector< IGesture::SP >		OverrideList;
typedef boost::shared_ptr<TelephonySpeechGesture>	SP;
typedef boost::weak_ptr<TelephonySpeechGesture>	WP;

	RTTI_DECL();

	//! Construction
	TelephonySpeechGesture() : m_pTTS( NULL ), m_pVoices( NULL ), SpeechGesture( "tts" )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Start();
	virtual bool Stop();
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

private:
	void StartSpeech();
	void OnVoices( Voices * a_pVoices );
	void OnSpeechData( Sound * a_pSound );
	void WaitOnBuffer();
	void OnSpeechDone();

	TextToSpeech *		m_pTTS;
	Telephony *         m_pTelephony;
	Voices *			m_pVoices;
	bool				m_bOverride;
	OverrideList		m_Overrides;
};


#endif //TELE_SPEECH_GESTURE_H