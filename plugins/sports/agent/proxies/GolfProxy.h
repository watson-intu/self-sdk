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

#ifndef SELF_GOLF_PROXY_H
#define SELF_GOLF_PROXY_H

#include "agent/proxies/IQuestionAnswerProxy.h"
#include "services/Golf/Golf.h"

class GolfProxy : public IQuestionAnswerProxy
{
public:
    RTTI_DECL();

    //! Construction
    GolfProxy();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IQuestionAnswer
	virtual void Start();
	virtual void Stop();
	virtual void AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback);

private:
    //! Data
    Golf *    m_pGolf;

    //! Private request class
    class GolfRequest 
    {
    public:
        GolfRequest(GolfProxy * a_pGolfProxy, std::string a_Question, Delegate<const Json::Value &>);
        void SendRequest();
        void OnResponse(const Json::Value & json);

    private:
        GolfProxy *                     m_pGolfProxy;
        std::string                     m_Question;
        Delegate<const Json::Value &>   m_Callback;
    };

};

#endif
