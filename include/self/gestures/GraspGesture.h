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

#ifndef GRASP_GESTURE_H
#define GRASP_GESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture grabs an object in the enviroment.
class SELF_API GraspGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	GraspGesture() 
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

protected:
	//! Data
	std::string m_Hand;
	bool m_Open;
};


#endif //GRASP_GESTURE_H
