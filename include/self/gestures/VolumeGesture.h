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

#ifndef VOLUME_GESTURE_H
#define VOLUME_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This is the base class for any gesture that changes the volume.
class SELF_API VolumeGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	VolumeGesture() : m_fChange( 0.0f ), m_fTargetVolume( 0.0f )
	{}
	VolumeGesture( const std::string & a_GestureId ) : IGesture( a_GestureId ),
	    m_fChange( 0.0f ), m_fTargetVolume( 0.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );

	//! Mutators
	void SetChange(float a_fChange)
	{
		m_fChange = a_fChange;
	}
	void SetTargetVolume(float a_fTargetVolume)
	{
		m_fTargetVolume = a_fTargetVolume;
	}

protected:
	//! Data
	float m_fChange;
	float m_fTargetVolume;
};


#endif //VOLUME_GESTURE_H
