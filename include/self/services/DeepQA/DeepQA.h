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

#ifndef SELF_DEEPQA_H
#define SELF_DEEPQA_H

#include "services/IService.h"

class DeepQA : public IService
{
public:
    RTTI_DECL();

    //! Construction
    DeepQA();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    void AskQuestion(const std::string & a_Input, Delegate<const Json::Value &> a_Callback,
        int a_NumAnswers = 1, bool a_bInferQuestion = false, Json::Value a_Evidence = Json::Value() );

};

#endif
