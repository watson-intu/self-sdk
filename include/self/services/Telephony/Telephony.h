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

#ifndef SELF_TELEPHONY_H
#define SELF_TELEPHONY_H

#include "utils/IWebClient.h"
#include "services/IService.h"
#include "SelfLib.h"			// include last always

//! This service interfaces with the telephony gateway to allow for sending & accepting phone calls and SMS messages.
class SELF_API Telephony : public IService
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<const Json::Value &>		OnCommand;
	typedef Delegate<const std::string &>		OnAudioOut;
	
	//! Construction
	Telephony();
	~Telephony();

	//! Accessors
	IWebClient *		GetConnection() const;
	const std::string &	GetAudioInFormat() const;
	const std::string &	GetAudioOutFormat() const;
	const std::string & GetMyNumber() const;

	//! Connect to the back-end, this will register and we will become available to receive phone calls. The provided
	//! Callback will be invoke when a call is incoming, the user must call Answer() to answer the incoming call.
	bool	Connect( 
				const std::string & a_GroupId, 
				const std::string & a_SelfId,
				OnCommand a_OnCommand, 
				OnAudioOut a_OnAudioOut );
	//! Make outgoing call.
	bool	Dial( const std::string & a_Number );
	//! Answer an incoming call, returns true on success.
	bool	Answer();
	//! Hang up current call.
	bool	HangUp();
	//! Send a SMS message
	bool	Text( const std::string & a_Number, 
				const std::string & a_Message );
	//! Disconnect from the back-end..
	bool	Disconnect();

	//! Send binary audio data up to the gateway, the format of the audio must match the format 
	//! specified by GetAudioFormat(), usually audio/L16;rate=16000
	void	SendAudioIn( const std::string & a_Audio );

private:
	//! Data
	OnCommand			m_OnCommand;
	OnAudioOut			m_OnAudioOut;
	IWebClient *		m_pConnection;
	bool				m_bConnected;
	bool				m_bInCall;
	std::string			m_AudioInFormat;
	std::string			m_AudioOutFormat;
	std::string			m_MyNumber;
	TimerPool::ITimer::SP
						m_spReconnectTimer;

	//! IWebClient callbacks
	void				OnListenMessage( IWebSocket::FrameSP a_spFrame );
	void				OnListenState( IWebClient * a_pClient );
	void				OnListenData( IWebClient::RequestData * a_pData );

	void				OnReconnect();
};

//----------------------------

inline IWebClient * Telephony::GetConnection() const
{
	return m_pConnection;
}

inline const std::string & Telephony::GetAudioInFormat() const
{
	return m_AudioInFormat;
}

inline const std::string & Telephony::GetAudioOutFormat() const
{
	return m_AudioOutFormat;
}

inline const std::string & Telephony::GetMyNumber() const
{
	return m_MyNumber;
}

#endif //SELF_TELEPHONY_H