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

#ifndef SELF_SOUND_GESTURE_H
#define SELF_SOUND_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture wraps a sound effect to play back
class SELF_API SoundGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	SoundGesture() : m_fVolume( 1.0f ), m_fPan( 0.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

	const std::string & GetSound() const
	{
		return m_Sound;
	}

	void SetSound( const std::string & a_Sound )
	{
		m_Sound = a_Sound;
	}

protected:
	//!Data
	std::string			m_Sound;		// path to the sound to play
	float				m_fVolume;
	float				m_fPan;
};


#endif //SELF_SOUND_GESTURE_H
