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

#ifndef SELF_IQUESTIONANSWER_PROXY_H
#define SELF_IQUESTIONANSWER_PROXY_H

#include "blackboard/QuestionIntent.h"

class SELF_API IQuestionAnswerProxy : public ISerializable, public boost::enable_shared_from_this<IQuestionAnswerProxy>
{
public:
    RTTI_DECL();

    //! Types
	typedef boost::shared_ptr<IQuestionAnswerProxy>		SP;
	typedef boost::weak_ptr<IQuestionAnswerProxy>		WP;

	//! Construction
	IQuestionAnswerProxy() : m_bPriorityFlag( true )
	{}
	virtual ~IQuestionAnswerProxy()
	{}

	//! ISerializable
	virtual void Serialize(Json::Value & json) 
	{
		json["m_bPriorityFlag"] = m_bPriorityFlag;
	}
	virtual void Deserialize(const Json::Value & json)
	{
		if ( json.isMember("m_bPriorityFlag") )
			m_bPriorityFlag = json["m_bPriorityFlag"].asBool();
	}

    //! Interface
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void AskQuestion( QuestionIntent::SP a_spQuestion, Delegate<const Json::Value &> a_Callback ) = 0;

	//! Accessors
	const bool HasPriority() const { return m_bPriorityFlag; }

private:
	//! Data
	bool	m_bPriorityFlag;
};

#endif // SELF_IQUESTIONANSWER_PROXY_H