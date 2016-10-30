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

#include "RemoteMicrophone.h"
#include "SelfInstance.h"

REG_OVERRIDE_SERIALIZABLE(Microphone, RemoteMicrophone);
REG_SERIALIZABLE( RemoteMicrophone );
RTTI_IMPL(RemoteMicrophone, Microphone);

bool RemoteMicrophone::OnStart()
{
	SelfInstance * pInstance = SelfInstance::GetInstance();
	if (pInstance != NULL)
	{
		ITopics * pTopics = pInstance->GetTopics();
		pTopics->RegisterTopic("audio-input", "audio/L16;rate=16000");
		pTopics->Subscribe("audio-input", DELEGATE(RemoteMicrophone, OnRemoteAudio, const ITopics::Payload &, this));
	}

	return true;
}

bool RemoteMicrophone::OnStop()
{
	SelfInstance * pInstance = SelfInstance::GetInstance();
	if (pInstance != NULL)
	{
		ITopics * pTopics = pInstance->GetTopics();
		pTopics->Unsubscribe("audio-input");
		pTopics->UnregisterTopic("audio-input");
	}
	return true;
}

void RemoteMicrophone::OnPause()
{
	m_Paused++;
}

void RemoteMicrophone::OnResume()
{
	m_Paused--;
}

void RemoteMicrophone::OnRemoteAudio( const ITopics::Payload & a_Payload )
{
	if (m_Paused <= 0 )
		SendData( new AudioData( a_Payload.m_Data, 16000, 1, 16 ) );
}

