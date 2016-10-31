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

#ifndef SELF_AGENTSOCIETY_H
#define SELF_AGENTSOCIETY_H

#include <list>

#include "IAgent.h"
#include "topics/TopicManager.h"
#include "utils/Factory.h"
#include "utils/RTTI.h"
#include "SelfLib.h"		// include last

class SelfInstance;
class ProxySensor;

class SELF_API AgentSociety
{
public:
    //! Types
    typedef std::vector< IAgent::SP >	AgentList;
    typedef Factory< IAgent >			AgentFactory;

    //! Construction
    AgentSociety();
    ~AgentSociety();

    //! Accessors
    const AgentList &		    GetAgentList() const;

	//! Start this manager
    bool						Start();
    //! Stop this manager.
    bool						Stop();

	//! Add the agent to this society, it takes ownership of the object if accepted.
	bool					AddAgent(const IAgent::SP & a_spAgent, bool a_bOverride = false);
	//! Remove a agent from this society.
	bool					RemoveAgent(const IAgent::SP & a_spAgent);

	bool					FindAgentsByName(const std::string & a_Name, std::vector<IAgent::SP> & a_Overrides);


	void OnAgentOverride(IAgent * a_pAgent);
	void OnAgentOverrideEnd(IAgent * a_pAgent);

	template<typename T>
	T * GetAgent() const
	{
		for( AgentList::const_iterator iAgent = m_Agents.begin(); iAgent != m_Agents.end(); ++iAgent )
		{
			T * pAgent = DynamicCast<T>( *iAgent );
			if ( pAgent != NULL )
				return pAgent;
		}
		return NULL;
	}

private:
    //! Data
    bool						m_bActive;
    AgentList				    m_Agents;
	TopicManager *				m_pTopicManager;

	//! Callbacks
	void					OnSubscriber(const ITopics::SubInfo & a_Info);
	void					OnAgentEvent(const ITopics::Payload & a_Payload);

};

inline const AgentSociety::AgentList & AgentSociety::GetAgentList() const
{
    return m_Agents;
}

#endif //SELF_AGENTSOCIETY_H