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

#ifndef SELF_MERGES_AND_ACQUISITIONS_PROXY_H
#define SELF_MERGES_AND_ACQUISITIONS_PROXY_H

#include "agent/proxies/IQuestionAnswerProxy.h"
#include "services/MergersAndAcquisitions.h"

class MergersAndAcquisitionsProxy : public IQuestionAnswerProxy
{
public:
    RTTI_DECL();

    //! Construction
    MergersAndAcquisitionsProxy();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IQuestionAnswer
    virtual void Start();
    virtual void Stop();
    virtual void AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback);

private:
    //! Data
    MergersAndAcquisitions *    m_pMergersAndAcquisitions;

    //! Private request class
    class MergersAndAcquisitionsRequest
    {
    public:
        MergersAndAcquisitionsRequest(MergersAndAcquisitionsProxy * m_pMergersAndAcquisitionsProxy, const std::string & a_Question, Delegate<const Json::Value &>);
        void OnResponse(const std::string & json);

    private:
        MergersAndAcquisitionsProxy *                       m_pMergersAndAcquisitionsProxy;
        std::string                                         m_Question;
        Delegate<const Json::Value &>                       m_Callback;
    };

};

#endif // SELF_MERGES_AND_ACQUISITIONS_PROXY_H