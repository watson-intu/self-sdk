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

#ifndef SELF_PROXY_AGENT_H
#define SELF_PROXY_AGENT_H

#include "IAgent.h"
#include "SelfLib.h"

//! This sensor class is used to represent a remote sensor that is running outside this self instance
class SELF_API ProxyAgent : public IAgent
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr< ProxyAgent >		SP;
	typedef boost::weak_ptr< ProxyAgent >		WP;

	//! Construction
	ProxyAgent(const std::string & a_AgentId,
		const std::string & a_InstanceId,
		bool a_bOverride,
		const std::string & a_Origin);
	ProxyAgent();
	~ProxyAgent();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface	
	virtual bool OnStart();
	virtual bool OnStop();
	virtual const std::string & GetAgentName() const
	{
		return m_AgentId;
	}

	void SendEvent(const std::string & a_EventName);

	const std::string & GetInstanceId() const
	{
		return m_InstanceId;
	}

	const std::string & GetOrigin() const
	{
		return m_Origin;
	}

	const std::string & GetAgentId() const
	{
		return m_AgentId;
	}

	

protected:
	//! Data
	std::string			m_Origin;
	std::string			m_InstanceId;
	std::string			m_AgentId;
	bool				m_bOverride;
};

#endif // SELF_PROXY_AGENT_H
