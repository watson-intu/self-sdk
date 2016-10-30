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

#ifndef SELF_CONVERSATION_PROXY_H
#define SELF_CONVERSATION_PROXY_H

#include "ITextClassifierProxy.h"
#include "blackboard/Text.h"

class Conversation;
struct ConversationResponse;

class SELF_API ConversationProxy : public ITextClassifierProxy
{
public:
	RTTI_DECL();

	//! Construction
	ConversationProxy();
	virtual ~ConversationProxy();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ITextClassifierProxy interface
	virtual void Start();
	virtual void Stop();

	virtual void ClassifyText( Text::SP a_spText, Delegate<ClassifyResult *> a_Callback );

private:
	//! Data
	Conversation *					m_pConversation;
	Json::Value						m_Context;						// current context
	std::string						m_WorkspaceId;					// ID of the workspace to use

	//! Helper request object
	class Request
	{
	public:
		Request( ConversationProxy * a_pConversationProxy, Text::SP a_spText, Delegate<ClassifyResult *> a_Callback );

		void OnTextClassified( ConversationResponse * a_pResponse );

	private:
		ConversationProxy *                 m_pProxy;
		Text::SP                         	m_spText;
		Delegate<ClassifyResult *>     		m_Callback;
	};
};

#endif // SELF_NLC_PROXY_H