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

#ifndef SELF_UPDATEAGENT_H
#define SELF_UPDATEAGENT_H

#include <list>

#include "IAgent.h"
#include "utils/Factory.h"
#include "utils/ThreadPool.h"
#include "services/PackageStore/PackageStore.h"
#include "SelfInstance.h"
#include "blackboard/Confirm.h"


#include "SelfLib.h"


//! Forward Declarations
class SelfInstance;
class PackageStore;

class SELF_API UpdateAgent : public IAgent
{
public:
	RTTI_DECL();

	//! Construction
	UpdateAgent() : m_bDownloadChecked(false), m_bAllowRecommendedDownload(false)
	{
		m_pPackageStore = SelfInstance::GetInstance()->GetService<PackageStore>();
	}

	void OnManualUpgrade();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Data
	TimerPool::ITimer::SP		m_spCheckVersionTimer;
	PackageStore *				m_pPackageStore;

	//! Event Handlers
	void StartTimer();
	void OnVersionRequest();
	void OnGetVersions(const Json::Value & response);

	void OnDownloadPackage(const std::string & downloadResponse);
	void OnConfirmDownload(Confirm::SP a_spConfirm);
	void OnGetManualVersions(const Json::Value & response);

	//! Data
	bool		m_bDownloadChecked;
	bool		m_bAllowRecommendedDownload;
	float		m_fUpdateCheckDelay;

	std::string	m_LastVersionConfirmed;
	std::string	m_SelfPackageName;
	std::string m_UpdatePackageVersion;
	std::string	m_FoundUpdateResponse;
	std::string	m_InstallationCompleteResponse;
};

#endif //SELF_UPDATEAGENT_H