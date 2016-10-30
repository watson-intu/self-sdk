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

#ifndef SELF_REQUESTAGENT_H
#define SELF_REQUESTAGENT_H

#include <list>

#include "IAgent.h"
#include "utils/Factory.h"
#include "blackboard/Entity.h"
#include "blackboard/Image.h"

#include "SelfLib.h"


//! Forward Declarations
class SelfInstance;
class RequestIntent;

class SELF_API RequestAgent : public IAgent
{
public:
	RTTI_DECL();

	//! ISerializable interface
	void Serialize( Json::Value & json );
	void Deserialize( const Json::Value & json );

    //! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Event Handlers
    void OnRequestIntent(const ThingEvent & a_ThingEvent);
	void OnEntity(const ThingEvent & a_ThingEvent);
	void OnGoalState( const ThingEvent & a_Event );

	//! Data
	std::vector<std::string>							m_RequestFailedText;
	std::vector<std::string>    						m_IdentifyObjectText;
	std::vector<std::string>							m_LowImageConfidenceText;
	std::list<Entity::SP>								m_EntityHistory;
	double												m_fMinimumImageConfidence;
};

#endif //SELF_REQUESTAGENT_H