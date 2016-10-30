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

#ifndef SELF_EXCEPTION_H
#define SELF_EXCEPTION_H

//! Class for exceptions throw by the SELF 
class SelfException 
{
public:
	SelfException( const char * a_pMessage )
	{
		m_pMessage = a_pMessage;
	}

	const char * Message() const
	{
		return m_pMessage;
	}

private:
	const char * m_pMessage;
};

#endif
