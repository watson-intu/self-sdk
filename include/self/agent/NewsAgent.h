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

#ifndef SELF_NEWSAGENT_H
#define SELF_NEWSAGENT_H

#include "IAgent.h"
#include "services/Alchemy/Alchemy.h"
#include "blackboard/NewsIntent.h"
#include "SelfLib.h"

class SELF_API NewsAgent : public IAgent
{
public:
	RTTI_DECL();

	NewsAgent() : m_NumberOfArticles( 10 )
	{}

	//! ISerializable interface
	void Serialize(Json::Value & json);
	void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Data
	Alchemy *		m_pAlchemy;
	NewsIntent::SP  m_spActive;
	int				m_NumberOfArticles;

	//! Event Handlers
	void                    OnNewsIntent(const ThingEvent & a_ThingEvent);
	void                    OnNewsData(const Json::Value & a_Callback);
};

#endif //SELF_NEWSAGENT_H
