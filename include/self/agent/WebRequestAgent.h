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

#ifndef SELF_WEBREQUESTAGENT_H
#define SELF_WEBREQUESTAGENT_H

#include "IAgent.h"
#include "utils/IWebClient.h"
#include "blackboard/WebRequest.h"

class SkillInstance;

class SELF_API WebRequestAgent : public IAgent
{
public:
    RTTI_DECL();

    WebRequestAgent() : m_pClient(NULL)
    {}

	~WebRequestAgent();

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:

    //! Types
    typedef std::list<WebRequest::SP>		RequestList;

    //! Data
    WebRequest::SP  m_spActive;
    RequestList     m_Requests;
    IWebClient *    m_pClient;

    //! Event Handlers
    void OnWebRequest(const ThingEvent & a_ThingEvent);
    void ExecuteRequest(WebRequest::SP a_spWebRequest);
    void OnResponse(IWebClient::RequestData * a_pResponse);
    void OnState(IWebClient * a_pConnector);
};

#endif //SELF_WEBREQUESTAGENT_H
