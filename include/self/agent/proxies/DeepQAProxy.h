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

#ifndef SELF_DEEPQA_PROXY_H
#define SELF_DEEPQA_PROXY_H

#include "IQuestionAnswerProxy.h"
#include "services/DeepQA/DeepQA.h"

class DeepQAProxy : public IQuestionAnswerProxy 
{
public:
    RTTI_DECL();

    //! Construction
    DeepQAProxy();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IQuestionAnswer
	virtual void Start();
	virtual void Stop();
	virtual void AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback);
    
private:
    //! Data
    DeepQA *                    m_pDeepQA;
	std::string					m_ServiceId;
	std::string					m_PipelineType;
    float                       m_fMinConfidence;
    std::vector<std::string>    m_LowConfidenceResponses;

    //! Private request class
    class DeepQARequest 
    {
    public:
        DeepQARequest( DeepQAProxy * a_pDeepQAProxy, std::string a_Question, Delegate<const Json::Value &> );
        void SendRequest();
        void OnResponse(const Json::Value & json);

    private:
        DeepQAProxy *                       m_pDeepQAProxy;
        std::string                         m_Question;
        Delegate<const Json::Value &>       m_Callback;
    };

};

#endif
