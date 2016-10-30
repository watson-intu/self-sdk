/**
* Copyright 2016 IBM Corp. All Rights Reserved.
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

#ifndef ISERVICE_H
#define ISERVICE_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/atomic.hpp"

#include "utils/DataCache.h"
#include "utils/ISerializable.h"
#include "utils/Delegate.h"
#include "utils/ThreadPool.h"
#include "utils/TimerPool.h"
#include "utils/ServiceConfig.h"
#include "utils/WatsonException.h"
#include "utils/IWebClient.h"
#include "WDCLib.h"			// include last always

//! This is the base class for a remote service.
class WDC_API IService : public ISerializable, public boost::enable_shared_from_this<IService>
{
public:
	RTTI_DECL();

	//! Forward declaration
	class Request;
	struct ServiceStatus;

	//! Data
	static boost::atomic<int> sm_Timeouts;

	//! Types
	typedef boost::shared_ptr<IService>		SP;
	typedef boost::weak_ptr<IService>		WP;

	typedef IWebClient::Headers				Headers;
	typedef Delegate<Request *>				ResponseCallback;
	typedef Delegate<const Json::Value &>	JsonResponseCallback;
	typedef Delegate<const std::string &>	DataResponseCallback;

	//! Callback function type invoked after service status check
	typedef Delegate<const ServiceStatus &>	ServiceStatusCallback;

	//! This struct is passed into a request to enable local cached requests
	struct CacheRequest
	{
		CacheRequest()
		{}
		CacheRequest(const std::string & a_CacheName, const std::string & a_Id) :
			m_CacheName(a_CacheName), m_Id(a_Id)
		{}
		CacheRequest(const std::string & a_CacheName, unsigned int a_Id) :
			m_CacheName(a_CacheName), m_Id(StringUtil::Format("%8.8x", a_Id))
		{}

		std::string			m_CacheName;
		std::string			m_Id;
	};

	//! REST request object for this service.
	class WDC_API Request
	{
	public:
		//! Types
		typedef IService::Headers		Headers;

		//! Construction
		Request( const std::string & a_URL,
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			ResponseCallback a_Callback,
			float a_fTimeout = 30.0f );
		Request(IService * a_pService,
			const std::string & a_Parameters,		// additional data to append onto the endpoint
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			ResponseCallback a_Callback,
			CacheRequest * a_CacheReq = NULL,
			float a_fTimeout = 30.0f );

		virtual ~Request()
		{
			delete m_pClient;
			delete m_pCachedReq;
		}

		IService * GetService() const
		{
			return m_pService;
		}
		bool IsComplete() const
		{
			return m_Complete;
		}
		bool IsError() const
		{
			return m_Error;
		}
		const std::string & GetResponse() const
		{
			return m_Response;
		}

	protected:
		//! HTTP callbacks
		void OnState( IWebClient * a_pClient );
		void OnResponseData( IWebClient::RequestData * a_pResponse );
		void OnLocalResponse();
		void OnTimeout();

		//! Data
		IService *			m_pService;
		IWebClient *		m_pClient;
		std::string			m_Body;
		std::string			m_Response;
		bool				m_Complete;
		bool				m_Error;
		ResponseCallback	m_Callback;
		CacheRequest *		m_pCachedReq;

		TimerPool::ITimer::SP
							m_spTimeoutTimer;
		bool				m_bDelete;

		double				m_CreateTime;
		double				m_StartTime;
	};

	//! This class can be used when the expected response will be JSON..
	class RequestJson : public Request
	{
	public:
		RequestJson(const std::string & a_URL,
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			JsonResponseCallback a_Callback,
			float a_fTimeout = 30.0f ) :
			m_Callback( a_Callback ),
			Request( a_URL, a_RequestType, a_Headers, a_Body, 
				DELEGATE( RequestJson, OnResponse, Request *, this ), a_fTimeout)
		{}

		RequestJson(IService * a_pService,
			const std::string & a_Parameters,		// additional data to append onto the endpoint
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			JsonResponseCallback a_Callback,
			CacheRequest * a_CacheReq = NULL,
			float a_fTimeout = 30.0f ) :
			m_Callback(a_Callback),
			Request(a_pService, a_Parameters, a_RequestType, a_Headers, a_Body,
				DELEGATE(RequestJson, OnResponse, Request *, this), a_CacheReq, a_fTimeout )
		{}

	private:
		void OnResponse(IService::Request * a_pRequest)
		{
			Json::Value root;
			if (! a_pRequest->IsError() )
			{
				if (!Json::Reader(Json::Features::strictMode()).parse(m_Response, root))
				{
					Log::Error("RequestJson", "Failed to parse JSON response: %s", m_Response.c_str());
					root = Json::Value::null;
				}
			}

			if (m_Callback.IsValid())
			{
				m_Callback(root);
				m_Callback.Reset();
			}
		}
		JsonResponseCallback	m_Callback;
	};

	//! This request is used to retrieve raw byte data from the server.
	class RequestData : public Request
	{
	public:
		RequestData(const std::string & a_URL,
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			DataResponseCallback a_Callback,
			float a_fTimeout = 30.0f ) :
			m_Callback(a_Callback),
			Request(a_URL, a_RequestType, a_Headers, a_Body,
				DELEGATE(RequestData, OnResponse, Request *, this), a_fTimeout)
		{}

		RequestData(IService * a_pService,
			const std::string & a_Parameters,		// additional data to append onto the endpoint
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			DataResponseCallback a_Callback,
			CacheRequest * a_CacheReq = NULL,
			float a_fTimeout = 30.0f ) :
			m_Callback(a_Callback),
			Request(a_pService, a_Parameters, a_RequestType, a_Headers, a_Body,
				DELEGATE(RequestData, OnResponse, Request *, this), a_CacheReq, a_fTimeout )
		{}

	private:
		void OnResponse(IService::Request * a_pRequest)
		{
			if (! a_pRequest->IsError() )
			{
				if (m_Callback.IsValid())
				{
					m_Callback(m_Response);
					m_Callback.Reset();
				}
			}
			else
			{
				if (m_Callback.IsValid())
				{
					m_Callback("");
					m_Callback.Reset();
				}
			}
		}
		DataResponseCallback	m_Callback;
	};

	//! Requests the object for type T, this will attempt to deserialize the 
	//! returned json into the given object.
	template<typename T>
	class RequestObj : public Request
	{
	public:
		RequestObj(const std::string & a_URL,
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			Delegate<T *> a_Callback,
			float a_fTimeout = 30.0f ) :
			m_Callback(a_Callback),
			Request(a_URL, a_RequestType, a_Headers, a_Body,
				DELEGATE(RequestObj, OnResponse, Request *, this), a_fTimeout )
		{}

		RequestObj(IService * a_pService,
			const std::string & a_Parameters,		// additional data to append onto the endpoint
			const std::string & a_RequestType,		// type of request GET, POST, DELETE
			const Headers & a_Headers,				// additional headers to add to the request
			const std::string & a_Body,				// the body to send if any
			Delegate<T *> a_Callback,
			CacheRequest * a_CacheReq = NULL,
			float a_fTimeout = 30.0f ) :
			m_Callback(a_Callback),
			Request(a_pService, a_Parameters, a_RequestType, a_Headers, a_Body,
				DELEGATE(RequestObj, OnResponse, Request *, this), a_CacheReq, a_fTimeout )
		{}

	private:
		void OnResponse(IService::Request * a_pRequest)
		{
			T * pObject = NULL;
			if (! a_pRequest->IsError() )
			{
				pObject = new T();
				if ( ISerializable::DeserializeObject( m_Response, pObject) == NULL )
				{
					Log::Error( "RequestObj", "Failed to deserialize object: %s", m_Response.c_str() );
					delete pObject;
					pObject = NULL;
				}
			}

			if (m_Callback.IsValid())
			{
				m_Callback(pObject);
				m_Callback.Reset();
			}
		}
		Delegate<T *>	m_Callback;
	};

	//! This struct is used as a parameter for GetServiceStatus() callback
	struct ServiceStatus
	{
		ServiceStatus(const std::string & a_ServiceId, bool a_Status)
			: m_ServiceId(a_ServiceId)
			, m_Status(a_Status)
		{}

		std::string m_ServiceId;
		bool m_Status;
	};

	//! Constructions
	IService(const std::string & a_ServiceId);

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	const Headers & GetHeaders() const
	{
		return m_Headers;
	}
	const std::string & GetServiceId() const
	{
		return m_ServiceId;
	}
	const ServiceConfig * GetConfig() const
	{
		if (m_pConfig == NULL )
			throw WatsonException( "Service config is NULL, make sure you invoke Start()." );
		return m_pConfig;
	}
	bool IsCacheEnabled() const
	{
		return m_bCacheEnabled;
	}

	//! Start this service, returns true on success.
	virtual bool Start();
	//! Stop this service, returns true on success.
	virtual bool Stop();

	void SetCacheEnabled(bool a_bEnabled)
	{
		m_bCacheEnabled = a_bEnabled;
	}

	//! Update the default headers of this service, this will change
	//! the headers of any REST request made using this service.
	void SetHeaders( const Headers & a_Headers )
	{
		m_Headers = a_Headers;
	}

	//! Check if the service is up or down, and invoke the callback with the current
	//! state of the service once determined.
	virtual void GetServiceStatus(ServiceStatusCallback a_Callback);
	//! This is invoked when the ServiceConfig object has been modified for this service
	virtual void OnConfigModified();

protected:
	//! Types
	typedef std::map<std::string, DataCache::SP>		DataCacheMap;

	//! Data
	std::string		m_ServiceId;
	const ServiceConfig *	
					m_pConfig;
	Headers			m_Headers;				// default headers for this service
	
	bool			m_bCacheEnabled;
	unsigned int	m_MaxCacheSize;
	double			m_MaxCacheAge;
	float			m_RequestTimeout;
	DataCacheMap	m_DataCache;

	boost::atomic<int>
					m_RequestsPending;

	DataCache *		GetDataCache(const std::string & a_Type);
	bool			GetCachedResponse(const std::string & a_CacheName, const std::string & a_Id,std::string & a_Response);
	bool			GetCachedResponse(const std::string & a_CacheName, unsigned int a_Id, std::string & a_Response);
	void			PutCachedResponse(const std::string & a_CacheName,
						const std::string & a_Id,
						const std::string & a_Response);
};

const static IService::Headers NULL_HEADERS;
const static std::string EMPTY_STRING;

#endif

