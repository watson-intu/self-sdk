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

#ifndef SELF_LOOKUP_AGENT_H
#define SELF_LOOKUP_AGENT_H

#include "agent/IAgent.h"
#include "blackboard/LookupIntent.h"
#include "services/RetrieveAndRank/RetrieveAndRank.h"
#include "utils/IWebClient.h"
#include "SelfLib.h"

class SkillInstance;

class LookupAgent : public IAgent
{
public:
    RTTI_DECL();

    LookupAgent();

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    typedef std::list<LookupIntent::SP>		LookupList;
	typedef IWebClient::Headers		Headers;
    //! Event Handlers
    void                    OnLookupIntent(const ThingEvent & a_ThingEvent);
    void                    ExecuteRequest(LookupIntent::SP a_pWEX);
    void                    OnMessage( RetrieveAndRankResponse * a_pRetrieveAndRankResponse );

    LookupIntent::SP	        m_spActive;
    LookupList					m_LookupRequests;
	std::string					m_KeyPrefix;
	std::string					m_URL;
	Headers						m_Headers;

};

#endif //SELF_WEXAGENT_H
