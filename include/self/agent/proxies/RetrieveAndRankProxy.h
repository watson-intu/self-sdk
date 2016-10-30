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

#ifndef SELF_RETRIEVEANDRANKPROXY_H
#define SELF_RETRIEVEANDRANKPROXY_H

#include "agent/proxies/IQuestionAnswerProxy.h"
#include "services/RetrieveAndRank/RetrieveAndRank.h"

class RetrieveAndRankProxy : public IQuestionAnswerProxy
{
public:
    RTTI_DECL();

    //! Construction
    RetrieveAndRankProxy();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IQuestionAnswer
    virtual void Start();
    virtual void Stop();
    virtual void AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback);

private:
    //! Data
    RetrieveAndRank *    m_pRetrieveAndRank;

    //! Private request class
    class RetrieveAndRankRequest
    {
    public:
        RetrieveAndRankRequest(RetrieveAndRankProxy * m_pRetrieveAndRankProxy, const std::string & a_Question, Delegate<const Json::Value &>);
        void OnResponse(RetrieveAndRankResponse * a_pRetrieveAndRankResponse);

    private:
        RetrieveAndRankProxy *                       m_pRetrieveAndRankProxy;
        std::string                                  m_Question;
        Delegate<const Json::Value &>                m_Callback;
    };


};

#endif //SELF_RETRIEVEANDRANKPROXY_H
