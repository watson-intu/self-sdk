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

#ifndef SELF_WATSON_AVATAR_GESTURE_H
#define SELF_WATSON_AVATAR_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture waits for a specified period of time
class SELF_API WatsonAvatarGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	WatsonAvatarGesture()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap & a_Params);
	virtual bool Abort();

private:
	//! Data
	std::string m_StateId;
};

#endif //SELF_WAITGESTURE_H