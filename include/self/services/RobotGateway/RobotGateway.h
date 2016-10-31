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

#ifndef SELF_ROBOT_GATEWAY_H
#define SELF_ROBOT_GATEWAY_H

#include "blackboard/ThingEvent.h"
#include "services/IService.h"
#include "DataModels.h"
#include "utils/TimerPool.h"
#include "SelfLib.h"			// include last always

class DialogContainer;
class SkillInstance;

//! This service wraps the RobotGateway BlueMix application. Depending on the configuration,
//! some of these calls may go directly to BlueMix.
class SELF_API RobotGateway : public IService, public ILogReactor
{
public:
	RTTI_DECL();

	//! Construction
	RobotGateway();
	~RobotGateway();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
	virtual bool Stop();
	virtual void GetServiceStatus(ServiceStatusCallback a_Callback);

	//! ILogReactor interface
	virtual void Process(const LogRecord & a_Record);

	//! Accessors
	const std::string & GetOrgAdmins() const
	{
		return m_OrgAdminList;
	}

	//! Register embodiment on gateway
	void RegisterEmbodiment( Delegate<const Json::Value &> a_Callback );
	//! Send e-mail to gateway
	void SendEmail( const std::string & a_To,
		const std::string & a_Subject, 
		const std::string & a_Message, Delegate<Request *> a_Callback);
	//! Send backtrace from previous crash
	void SendBacktrace( const std::string & a_BT );
	//! Send heart-beat to the gateway
	void Heartbeat();
	//! Get information about our current organization.
	void GetOrganization( Delegate<const Json::Value &> a_Callback );
	//! Get organization admin list.
	void GetOrgAdminList( Delegate<const Json::Value &> a_Callback );
	//! Get current parent.
	void GetParent( const std::string & a_ParentId, Delegate<const Json::Value &> a_Callback );
	//! Get the current configuration data store in the gateway for the provided Robot Key and mac ID. The user
	//! is responsible for the ServieList object returned in the callback, they must call delete on the object
	//! when they are done.
	void GetServices( Delegate<ServiceList *> a_Callback );
	//! Authenticate if another self instance is allowed to connect into this self
	void Authenticate( const std::string & a_Token, 
		Delegate<const Json::Value &> a_Callback );

private:
	//! This class is responsible for checking whether the service is available or not
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(RobotGateway* a_pRbgService, ServiceStatusCallback a_Callback);

	private:
		RobotGateway* m_pRbgService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(ServiceList* a_ServiceList);
	};

	//! Data
	TimerPool::ITimer::SP 
						m_spConfigTimer;
	std::string			m_GroupId;
	std::string			m_RobotName;
	std::string         m_RobotType;
	std::string			m_OrganizationId;
	std::string			m_MacId;
	std::string			m_SelfVersion;
	std::string         m_EmbodimentToken;
	std::string			m_EmbodimentId;
	std::string			m_EmbodimentName;
	std::string         m_OrgAdminList;

	bool				m_bApplyRemoteConfigs;
	bool				m_bApplyParentHost;
	bool 				m_ConfigsFetched;

	double				m_PersistLogInterval;			// how often to upload persisted logs
	float				m_HeartBeatInterval;
	LogLevel			m_PersistLogLevel;				// what level to persist
	std::vector<std::string>
						m_PersistLogFilter;				// array of sub-system to persist to the gateway
	std::list<std::string>
						m_PersistLogs;					// list of items to persist to the gatewaY
	double              m_OldestLogTime;
	double              m_NewestLogTime;
	TimerPool::ITimer::SP
						m_spPersistLogTimer;			// timer for persisting logs to the backend
	TimerPool::ITimer::SP
						m_spHeartbeatTimer;				// Timer to hit robot gateway's heartbeat endpoint

	//! Callbacks
	void UpdateConfig();
	void OnConfigured(ServiceList* a_pServiceList);
	void OnOrganization(const Json::Value & a_Response);
	void OnOrgAdminList(const Json::Value & a_Response);
	void OnParent(const Json::Value & a_Response);
	void OnRegisteredEmbodiment(const Json::Value & a_Response);
	void OnPersistLogs();
	void OnLogsPersisted(const Json::Value & a_Response);
	void OnBacktracePersisted(const Json::Value & a_Response);
	void OnHeartbeat(const Json::Value & response);

};

#endif //ROBOT_GATEWAY_H
