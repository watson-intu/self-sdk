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

#ifndef SELF_WAITGESTURE_H
#define SELF_WAITGESTURE_H

#include "utils/TimerPool.h"
#include "IGesture.h"
#include "SelfLib.h"

//! This gesture waits for a specified period of time
class SELF_API WaitGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Construction
	WaitGesture()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap & a_Params);
	virtual bool Abort();

private:

	//! TimerPool callback
	void TerminateExecution();

	//! Data
	GestureDelegate m_Callback;
	float m_fDelayInterval;
	TimerPool::ITimer::SP m_spWaitTimer;
};

#endif //SELF_WAITGESTURE_H