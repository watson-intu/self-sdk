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

#ifndef SELF_REMOTE_MICROPHONE_H
#define SELF_REMOTE_MICROPHONE_H

#include "topics/ITopics.h"
#include "sensors/Microphone.h"

class RemoteMicrophone : public Microphone
{
public:
	RTTI_DECL();

	//! Construction
	RemoteMicrophone() : m_Paused( 0 )
	{}

	//! ISensor interface
	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

	//! Callbacks
	void OnRemoteAudio(const ITopics::Payload & a_Payload);

protected:
	//! Data
	int m_Paused;
};

#endif
