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

#ifndef SPEECH_GESTURE_H
#define SPEECH_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture wraps the local speech sythesis so the self can speak
class SELF_API SpeechGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	SpeechGesture() 
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();
};


#endif //SPEECH_GESTURE_H
