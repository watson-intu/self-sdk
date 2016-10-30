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

#ifndef SELF_TENNIS_PROXY_H
#define SELF_TENNIS_PROXY_H

#include "agent/proxies/IQuestionAnswerProxy.h"
#include "services/Tennis/Tennis.h"

class TennisProxy : public IQuestionAnswerProxy
{
public:
    RTTI_DECL();

    //! Construction
    TennisProxy();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IQuestionAnswer
    virtual void AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback);
    virtual void Start();
    virtual void Stop();

private:
    //! Data
    Tennis *    m_pTennis;

    //! Private request class
    class TennisRequest
    {
    public:
        TennisRequest(TennisProxy * a_pTennisProxy, std::string a_Question, Delegate<const Json::Value &>);
        void SendRequest();
        void OnResponse(const Json::Value & json);

    private:
        TennisProxy *                     m_pTennisProxy;
        std::string                     m_Question;
        Delegate<const Json::Value &>   m_Callback;
    };

};

#endif
