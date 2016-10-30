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

#ifndef SELF_SMS_H
#define SELF_SMS_H

#include "services/IService.h"
#include "SelfLib.h"

class SELF_API SMS : public IService
{
public:
    RTTI_DECL();

	//! Types
	typedef Delegate<Request *>			SendCallback;

    //! Construction
    SMS();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    void Send( const std::string & a_Number, const std::string & a_Text,
		SendCallback a_Callback );

private:

    std::string m_FromNumber;
    std::string m_Key;

};

#endif //SELF_SMS_H
