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

#ifndef STATUS_GESTURE_H
#define STATUS_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture wraps a status indicator on the robot.
class SELF_API StatusGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	StatusGesture() : m_Red(1.0f), m_Green(1.0f), m_Blue(1.0f)
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap & a_Params);
	virtual bool Abort();

protected:
	//! Data
	float 								m_Red;
	float 								m_Green;
	float 								m_Blue;

	std::map<std::string, std::string>	m_StateMap;
};


#endif //STATUS_GESTURE_H
