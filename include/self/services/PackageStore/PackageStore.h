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

#ifndef SELF_PACKAGESTORE_H
#define SELF_PACKAGESTORE_H

#include "services/IService.h"
#include "DataModels.h"
#include "utils/TimerPool.h"
#include "SelfLib.h"			// include last always

class DialogContainer;

//! This service wraps the PackageStore application.
class SELF_API PackageStore : public IService
{
public:
	RTTI_DECL();

	//! Construction
	PackageStore();
	~PackageStore();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
	virtual bool Stop();
	virtual void GetServiceStatus(ServiceStatusCallback a_Callback);

	void DownloadPackage(const std::string & a_PackageId, const std::string & a_Version, Delegate<const std::string &> a_Callback);
	void GetVersions(const std::string & a_PackageId, Delegate<const Json::Value &> a_Callback);
	int VersionCompare(const std::string & a_FirstVersion, const std::string & a_SecondVersion);

private:
	//! This class is responsible for checking whether the service is available or not
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(PackageStore* a_pPkgStoreService, ServiceStatusCallback a_Callback);

	private:
		PackageStore* m_pPkgStoreService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(const Json::Value & response);
	};

};

#endif //SELF_PACKAGESTORE_H