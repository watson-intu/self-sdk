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

#ifndef SELF_NAMEAGENT_H
#define SELF_NAMEAGENT_H

#include "IAgent.h"
#include "blackboard/NameIntent.h"
#include "blackboard/Text.h"
#include "blackboard/Image.h"
#include "blackboard/Confirm.h"
#include "blackboard/Entity.h"

#include "SelfLib.h"

class SELF_API NameAgent : public IAgent
{
public:
	RTTI_DECL();

	NameAgent()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Callbacks
	void OnImage(const ThingEvent & a_ThingEvent);
	void OnNameIntent(const ThingEvent & a_ThingEvent);
	void OnLearnNewName(NameIntent::SP spNameIntent);
	void OnLearnNewPerson(NameIntent::SP spNameIntent);

	// If Alchemy could not recognize a proper name, then default to last word in statement
	void FindNonProperName(NameIntent::SP & spNameIntent);

	//! Data
	std::vector<std::string>		m_LearnNewPerson;
	std::vector<std::string>		m_LearnNewName;
	std::list<Image::SP>			m_ImageHistory;
	std::string						m_MeEntity;
	std::string						m_YouEntity;
};

#endif // SELF_NAMEAGENT_H