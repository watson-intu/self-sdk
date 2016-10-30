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

#ifndef SELF_SELFUPDATEGESTURE_H
#define SELF_SELFUPDATEGESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture updates the version of Self
class SELF_API SelfUpdateGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	SelfUpdateGesture()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap & a_Params);
	virtual bool Abort();
};


#endif //SELF_SELFUPDATEGESTURE_H
