/**
* Copyright 2015 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef WDC_DIALOG_H
#define WDC_DIALOG_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "services/IService.h"
#include "DataModels.h"
#include "WDCLib.h"		// include last always

class WDC_API Dialog : public IService
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<Dialogs *>				OnGetDialogs;
	typedef Delegate<const Json::Value &>	OnUploadDialog;
	typedef Delegate<const std::string &>	OnDownloadDialog;
	typedef Delegate<const Json::Value &>	OnDeleteDialog;
	typedef Delegate<const Json::Value &>	OnConverse;

	enum DialogFormat
	{
		XML,
		JSON,
		BINARY
	};

	//! Construction 
	Dialog();

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
	virtual void GetServiceStatus( ServiceStatusCallback a_Callback );

	//! Request a list of all available dialogs
	void GetDialogs(OnGetDialogs callback);
	//! Download the specified dialog.
	void DownloadDialog( const std::string & a_DialogId,
		OnDownloadDialog a_Callback, 
		DialogFormat a_Format = XML );
	//! Upload the specified dialog
	bool UploadDialog( const std::string & a_DialogName, 
		const std::string & a_DialogFilePath,
		OnUploadDialog a_Callback );
	void UploadDialog( const std::string & a_DialogName,
		const std::string & a_DialogFileName,
		const std::string & a_DialogData, 
		OnUploadDialog a_Callback );
	//! Delete the specified dialog.
	void DeleteDialog( const std::string & a_DialogId,
		OnDeleteDialog a_Callback );

	//! Converse with dialog system. 
	void Converse(
		const std::string & a_DialogId, 
		const std::string & a_Input,
		OnConverse a_Callback,
		int a_ConversationId = 0,
		int a_ClientId = 0,
		bool a_bUseCache = true );

private:
	//! Types
	typedef std::map<std::string,DataCache>		DataCacheMap;

	//! This class is responsible for checking whether the service is available or not
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(Dialog* a_pDlgService, ServiceStatusCallback a_Callback);

	private:
		Dialog* m_pDlgService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(Dialogs* a_pDialogs);
	};

	class ConverseReq
	{
	public:
		ConverseReq(Dialog * a_pDialog, 
			const std::string & a_DialogId,
			const std::string & a_Input,
			OnConverse a_Callback,
			int a_ConversationId = 0,
			int a_ClientId = 0,
			bool a_bUseCache = true );

	private:
		//! Callback
		void OnResponse(const Json::Value & a_Response);

		//! Data
		Dialog *		m_pDialog;
		std::string		m_DialogId;
		unsigned int	m_InputHash;
		OnConverse		m_Callback;
		bool			m_bUseCache;
	};

	//! Data
	double				m_MaxDialogAge;
};

#endif
