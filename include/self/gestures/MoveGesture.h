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

#ifndef MOVE_GESTURE_H
#define MOVE_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture moves the robot to a location in the enviroment.
class SELF_API MoveGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	MoveGesture() : m_fX( 0.0f ), m_fY( 0.0f ), m_fZ( 0.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

protected:
	//! Data
	float m_fX;
	float m_fY;
	float m_fZ;
};


#endif //GRASP_GESTURE_H
