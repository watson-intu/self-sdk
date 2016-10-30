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

#ifndef WATSON_AVATAR_H
#define WATSON_AVATAR_H

#include "services/IService.h"
#include "boost/asio.hpp"

#include "SelfLib.h"			// include last always

//! Service for changing the state of the graphical Watson avatar
class SELF_API WatsonAvatar : public IService 
{
public:
	RTTI_DECL();

	//! Construction
	WatsonAvatar();
	~WatsonAvatar();

	//! IService interface
	virtual bool Stop();

	//! Change the state of the avatar
	bool ChangeState( const std::string & a_NewState );

private:
	//! Callbacks
	void SendState( std::string a_NewState );

	//! Data
	boost::asio::ip::tcp::socket *		
								m_pSocket;
};

#endif //SELF_STREAMSTT_H
