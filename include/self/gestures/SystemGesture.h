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

#ifndef SYSTEM_GESTURE_H
#define SYSTEM_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This is the base class for any gesture that changes the system.
class SELF_API SystemGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	SystemGesture()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value &json);

	virtual void Deserialize(const Json::Value &json);

	//! IGesture interface
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap &a_Params);

protected:
	//! Data
	std::string m_SystemId;
};
#endif //SYSTEM_GESTURE_H
