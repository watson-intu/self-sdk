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

class SELF_API IAgent : public ISerializable, public boost::enable_shared_from_this<IAgent>
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
    IAgent() : m_eState(AS_STOPPED), m_pSociety( NULL ), m_Overrides( 0 )
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

	void SetState(State a_eState)
	{
		m_eState = a_eState;
	}

	void SetAgentSociety(AgentSociety * a_pSociety, bool a_bOverride);


	//! Accessors
	State GetState() const
	{
		return m_eState;
	}

	virtual const std::string & GetAgentName() const
	{
		return GetRTTI().GetName();
	}

	bool IsOverridden() const
	{
		return m_Overrides > 0;
	}

	
	bool IsActive() const
	{
		if (m_eState == AS_RUNNING)
			return true;

		return false;
	}

	void AddOverride();
	void RemoveOverride();

protected:
	//! Data
	State				m_eState;
	AgentSociety *		m_pSociety;
	int					m_Overrides;
	std::vector< SP >	m_Overriden;

};

#endif //SELF_IAGENT_H
