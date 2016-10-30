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

#ifndef SELF_INSTANCE_H
#define SELF_INSTANCE_H

#include <string>

#include "services/IService.h"
#include "agent/IAgent.h"
#include "sensors/ISensor.h"
#include "classifiers/IClassifier.h"
#include "extractors/IFeatureExtractor.h"
#include "topics/ITopics.h"
#include "utils/Config.h"
#include "utils/GetMac.h"

#include "SelfLib.h"			// include last

//! Forward declare
class BlackBoard;
class GestureManager;
class SkillManager;
class SensorManager;
class ClassifierManager;
class PlanManager;
class ThreadPool;
class FeatureManager;
class AgentSociety;
class TimerPool;
class TopicManager;

//! This class wraps most of the core systems, this is the main object to create to make a self instance.
class SELF_API SelfInstance : public Config
{
public:
	RTTI_DECL();

	//! Types
	typedef std::list<IAgent::SP>				AgentList;
	typedef std::list<ISensor::SP>				SensorList;
	typedef std::list<IFeatureExtractor::SP>	ExtractorList;
	typedef std::list<IClassifier::SP>			ClassifierList;
	typedef std::list<std::string>				FileList;
	typedef std::list<std::string>				PatternList;

	//! Static instance
	static SelfInstance * GetInstance();

	//! Construction
	SelfInstance(const std::string & a_LocalStoragePath);
	~SelfInstance();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	bool					IsActive() const;
	const std::string &		GetLocalStoragePath() const;
	TopicManager *			GetTopicManager() const;
	ITopics *				GetTopics() const;
	BlackBoard *			GetBlackBoard() const;
	GestureManager *		GetGestureManager() const;
	SkillManager *			GetSkillManager() const;
	SensorManager *			GetSensorManager() const;
	ClassifierManager *		GetClassifierManager() const;
	PlanManager *			GetPlanManager() const;
	ThreadPool *			GetThreadPool() const;
	FeatureManager *		GetFeatureManager() const;
	AgentSociety *			GetAgentSociety() const;
	TimerPool *				GetTimerPool() const;

	const std::string &		GetRobotKey() const;
	const std::string & 	GetOrganizationId() const;
	const std::string &		GetRobotName() const;
	const std::string &		GetBackendLanguage() const;
	const std::string &		GetRobotType() const;
	const std::string &		GetMacId() const;
	const std::string &		GetRobotUrl() const;
	const std::string &		GetRobotIp() const;
	const std::string &		GetLogoUrl() const;
	const AgentList &		GetAgentList() const;
	const SensorList &		GetSensorList() const;
	const ExtractorList &	GetExtractorList() const;
	const ClassifierList &	GetClassifierList() const;
	const std::string &		GetSelfVersion() const;
	const PatternList & 	GetPatternList() const;
	const FileList &		GetGestureFiles() const;
	const FileList &		GetSkillFiles() const;
	const FileList &		GetPlanFiles() const;
	const std::string & 	GetSelfIp() const;
	const std::string &     GetBearerToken() const;
	const std::string &		GetEmbodimentId() const;
	bool					UseDevVersion() const;

	//! Initialize this self instance.
	bool					Start();
	//! Updates all systems of this self-instance, processes any pending events
	//! and queued data. Returns false if instance is stopped.
	bool					Update();
	//! Stop this self instance.
	bool					Stop();
	//! Main entry point for this SelfInstace, this calls Start(), Update(), then Stop()
	int						Run();

	void SetBearerToken(const std::string & a_BearerToken);
	void SetEmbodimentId(const std::string & a_EmbodimentId);

	//! Returns a agent object by it's type.
	template<typename T>
	T * FindAgent() const
	{
		for (AgentList::const_iterator iAgent = m_Agents.begin(); iAgent != m_Agents.end(); ++iAgent)
		{
			T * pAgent = DynamicCast<T>((*iAgent).get());
			if (pAgent != NULL)
				return pAgent;
		}

		return NULL;
	}

	template<typename T>
	T * FindClassifier() const
	{
		for (ClassifierList::const_iterator iClassifier = m_Classifiers.begin(); iClassifier != m_Classifiers.end(); ++iClassifier)
		{
			T * pClassifier = DynamicCast<T>((*iClassifier).get());
			if (pClassifier != NULL)
				return pClassifier;
		}

		return NULL;
	}

private:
	struct EmbodimentCreds : public ISerializable
	{
		RTTI_DECL();

		std::string m_BearerToken;
		std::string m_GroupId;
		std::string m_OrgId;

		virtual void Serialize(Json::Value & json)
		{
			json["m_BearerToken"] = m_BearerToken;
			json["m_GroupId"] = m_GroupId;
			json["m_OrgId"] = m_OrgId;
		}

		virtual void Deserialize(const Json::Value & json)
		{
			if ( json.isMember("m_BearerToken") )
				m_BearerToken = json["m_BearerToken"].asString();
			if ( json.isMember("m_GroupId") )
				m_GroupId = json["m_GroupId"].asString();
			if ( json.isMember("m_OrgId") )
				m_OrgId = json["m_OrgId"].asString();
		}
	};

	//! Data
	static SelfInstance *	sm_pInstance;
	std::string				m_LocalStoragePath;
	bool					m_bActive;
	TopicManager *			m_pTopicManager;
	ITopics *				m_pTopics;
	BlackBoard *			m_pBlackBoard;
	GestureManager *		m_pGestureManager;
	SkillManager *			m_pSkillManager;
	SensorManager *			m_pSensorManager;
	ClassifierManager *		m_pClassifierManager;
	PlanManager *			m_pPlanManager;
	ThreadPool *			m_pThreadPool;
	FeatureManager *		m_pFeatureManager;
	AgentSociety *			m_pAgentSociety;
	TimerPool *				m_pTimerPool;

	std::string				m_RobotName;		// name for this local robot
	std::string				m_BackendLanguage;	// Language that used by the back-end
	std::string				m_RobotType;		// type of robot
	std::string				m_MacId;			// mac ID for the local robot
	std::string				m_RobotUrl;			// URL of the robot
	std::string				m_RobotIp;			// IP address of the robot, pulled from the URL
	std::string				m_SelfIp;			// Robot's IP
	std::string				m_EmbodimentId;		// our embodiment Id
	std::string				m_LogoUrl;
	std::string				m_SelfVersion;		// Version of Self currently running
	float                   m_fDefaultVolume;
	AgentList				m_Agents;			// list of available agents to create
	SensorList				m_Sensors;			// list of sensors to create
	ExtractorList			m_Extractors;		// list of feature extractors
	ClassifierList			m_Classifiers;
	FileList				m_GestureFiles;		// list of files containing all gestures
	FileList				m_SkillFiles;
	FileList				m_PlanFiles;
	PatternList				m_Interfaces;
	bool					m_bUseDevVersion;
	TimerPool::ITimer::SP	m_spSaveBodyTimer;
	bool					m_bSaveOnStop;

	EmbodimentCreds         m_EmbodimentCreds;  // struct containing embodiment credentials

	void HandlePublishBody( const ITopics::Payload & a_Body );
	void OnSaveBody();
};

inline bool SelfInstance::IsActive() const
{
	return m_bActive;
}

inline const std::string & SelfInstance::GetLocalStoragePath() const
{
	return m_LocalStoragePath;
}

inline TopicManager * SelfInstance::GetTopicManager() const
{
	return m_pTopicManager;
}

inline ITopics * SelfInstance::GetTopics() const
{
	return m_pTopics;
}

inline BlackBoard * SelfInstance::GetBlackBoard() const
{
	return m_pBlackBoard;
}

inline GestureManager * SelfInstance::GetGestureManager() const
{
	return m_pGestureManager;
}

inline SkillManager * SelfInstance::GetSkillManager() const
{
	return m_pSkillManager;
}

inline SensorManager * SelfInstance::GetSensorManager() const
{
	return m_pSensorManager;
}

inline ClassifierManager * SelfInstance::GetClassifierManager() const
{
	return m_pClassifierManager;
}

inline PlanManager * SelfInstance::GetPlanManager() const
{
	return m_pPlanManager;
}

inline ThreadPool * SelfInstance::GetThreadPool() const
{
	return m_pThreadPool;
}

inline FeatureManager * SelfInstance::GetFeatureManager() const
{
	return m_pFeatureManager;
}

inline AgentSociety * SelfInstance::GetAgentSociety() const
{
	return m_pAgentSociety;
}

inline TimerPool * SelfInstance::GetTimerPool() const
{
	return m_pTimerPool;
}

inline const std::string & SelfInstance::GetRobotKey() const
{
	return m_EmbodimentCreds.m_GroupId;
}

inline const std::string & SelfInstance::GetOrganizationId() const
{
	return m_EmbodimentCreds.m_OrgId;
}

inline const std::string & SelfInstance::GetRobotName() const
{
	return m_RobotName;
}

inline const std::string & SelfInstance::GetBackendLanguage() const
{
	return m_BackendLanguage;
}

inline const std::string & SelfInstance::GetRobotType() const
{
	return m_RobotType;
}

inline const std::string & SelfInstance::GetMacId() const
{
	return m_MacId;
}

inline const std::string & SelfInstance::GetRobotUrl() const
{
	return m_RobotUrl;
}

inline const std::string & SelfInstance::GetRobotIp() const
{
	return m_RobotIp;
}

inline const std::string & SelfInstance::GetLogoUrl() const
{
	return m_LogoUrl;
}

inline const std::string & SelfInstance::GetSelfVersion() const
{
	return m_SelfVersion;
}

inline const SelfInstance::AgentList & SelfInstance::GetAgentList() const
{
	return m_Agents;
}

inline const SelfInstance::SensorList & SelfInstance::GetSensorList() const
{
	return m_Sensors;
}

inline const SelfInstance::ExtractorList & SelfInstance::GetExtractorList() const
{
	return m_Extractors;
}

inline const SelfInstance::ClassifierList &	SelfInstance::GetClassifierList() const
{
	return m_Classifiers;
}

inline const SelfInstance::PatternList & SelfInstance::GetPatternList() const
{
	return m_Interfaces;
}

inline const SelfInstance::FileList & SelfInstance::GetGestureFiles() const
{
	return m_GestureFiles;
}

inline const SelfInstance::FileList & SelfInstance::GetSkillFiles() const
{
	return m_SkillFiles;
}

inline const SelfInstance::FileList & SelfInstance::GetPlanFiles() const
{
	return m_PlanFiles;
}

inline const std::string & SelfInstance::GetSelfIp() const
{
	return m_SelfIp;
}

inline const std::string & SelfInstance::GetBearerToken() const
{
	return m_EmbodimentCreds.m_BearerToken;
}

inline const std::string & SelfInstance::GetEmbodimentId() const
{
	return m_EmbodimentId;
}

inline bool SelfInstance::UseDevVersion() const
{
	return m_bUseDevVersion;
}

inline void SelfInstance::SetBearerToken(const std::string & a_BearerToken)
{
	m_EmbodimentCreds.m_BearerToken = a_BearerToken;
}

inline void SelfInstance::SetEmbodimentId(const std::string & a_EmbodimentId)
{
	m_EmbodimentId = a_EmbodimentId;
}

#endif
