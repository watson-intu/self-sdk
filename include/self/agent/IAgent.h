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

#ifndef SELF_IAGENT_H
#define SELF_IAGENT_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

#include "blackboard/ThingEvent.h"
#include "utils/ISerializable.h"
#include "SelfLib.h"				// include last

class AgentSociety;
class BlackBoard;

class SELF_API IAgent : public ISerializable
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<IAgent>		SP;
	typedef boost::weak_ptr<IAgent>			WP;

	enum State {
		AS_STOPPED,
		AS_RUNNING,
		AS_SUSPENDED
	};

	//! Construction
    IAgent() : m_eState(AS_STOPPED)
    {}
    virtual ~IAgent()
    {}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json) 
	{}
	virtual void Deserialize(const Json::Value & json)
	{}

    //! Interface
    virtual bool OnStart() = 0;						// invoked when this classifier is added to the manager
    virtual bool OnStop() = 0;						// invoked when this classifier is being removed or shutdown

	State GetState() const
	{
		return m_eState;
	}

	void SetState(State a_eState)
	{
		m_eState = a_eState;
	}

private:
	//! Data
	State		m_eState;
};

#endif //SELF_IAGENT_H
