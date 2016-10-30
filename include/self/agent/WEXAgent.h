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
#ifndef SELF_WEXAGENT_H
#define SELF_WEXAGENT_H


#include "IAgent.h"
#include "blackboard/WEXIntent.h"
#include "SelfLib.h"

class SkillInstance;

class SELF_API WEXAgent : public IAgent
{
public:
    RTTI_DECL();

    WEXAgent();

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    typedef std::list<WEXIntent::SP>		WEXList;
    //! Event Handlers
    void                    OnWexIntent(const ThingEvent & a_ThingEvent);
    void                    ExecuteRequest(WEXIntent::SP a_pWEX);
    void                    OnMessage( const std::string & a_Callback );

    WEXIntent::SP	        m_spActive;
    WEXList                 m_WexRequests;

};

#endif //SELF_WEXAGENT_H
