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

#include "NaoURLService.h"
#include <qi/signal.hpp>
#include <qi/type/proxysignal.hpp>
#include "qi/anyobject.hpp"


REG_SERIALIZABLE(NaoURLService);
RTTI_IMPL( NaoURLService, URLService);

#ifndef _WIN32
REG_OVERRIDE_SERIALIZABLE( URLService, NaoURLService);
#endif

NaoURLService::NaoURLService() : URLService(), 
	m_bTabletConnected(false), m_TabletCheckInterval(10.0), m_TabletDisplayTime(120.0), m_fBrightness(1.0)
{}

void NaoURLService::Serialize(Json::Value & json)
{
	URLService::Serialize(json);
	json["m_TabletCheckInterval"] = m_TabletCheckInterval;
	json["m_TabletDisplayTime"] = m_TabletDisplayTime;
	json["m_fBrightness"] = m_fBrightness;
}

void NaoURLService::Deserialize(const Json::Value & json)
{
	URLService::Deserialize(json);
	if ( json.isMember("m_TabletCheckInterval") )
		m_TabletCheckInterval = json["m_TabletCheckInterval"].asFloat();
	if ( json.isMember("m_TabletDisplayTime") )
		m_TabletDisplayTime = json["m_TabletDisplayTime"].asFloat();
	if ( json.isMember("m_fBrightness") )
		m_fBrightness = json["m_fBrightness"].asFloat();

	ConfigureTablet();
}

bool NaoURLService::Start() 
{
	// These will be pulled from gateway
	m_Security = "";
	m_SSID = "";
	m_Password = "";
	
	if (! IService::Start() )
		return false;
	// Configure tablet connection and start timer for check
	m_spTabletCheckTimer = TimerPool::Instance()->StartTimer( VOID_DELEGATE(NaoURLService, OnCheckConnection, this), 
		m_TabletCheckInterval, true, true);
	m_spTabletDisplayTimer = TimerPool::Instance()->StartTimer( VOID_DELEGATE(NaoURLService, OnDisplayLogo, this), 
		0, true, false); // Just to initialize timer
	OnCheckConnection();
	ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(NaoURLService, ReceiveData, void *, this), NULL);

	return true;
}

void NaoURLService::ReceiveData(void * arg)
{
	try
	{
		DoReceiveData(arg);
	}
	catch( const std::exception & ex )
	{
		Log::Error("NaoURLService", "Caught exception in DoReceiveData: %s", ex.what() );
	}
}

void NaoURLService::DoReceiveData(void * arg)
{
	if (NaoPlatform::Instance()->HasService("ALTabletService" ))
	{
		m_Tablet = NaoPlatform::Instance()->GetSession()->service("ALTabletService");
		if (m_Tablet)
		{
			m_Tablet.connect("onTouchDown", qi::AnyFunction::fromDynamicFunction(	boost::bind( &NaoURLService::OnTouchData, this, _1 ) ) );
		}
	}
}

void NaoURLService::SendURL( const Url::SP & a_spUrl, UrlCallback a_Callback )
{
	Log::Debug("NaoURLService", "Received in SendURL command.");
	if ( m_bTabletConnected )
	{
		// Example request URL
		// {"url":"http://WatsonLabs:IBM4you.@hilton.wayblazer.com/locations/washington-dc/search?q=Can%20you%20recommend%20a%20shell%20fish%20restaurant"}
		Headers headers;
		headers["Content-Type"] = "application/json";

		std::string url(a_spUrl->GetURL());
		size_t argStart = url.find_first_of('?');
		if (argStart != std::string::npos) {
			std::vector <std::string> args;
			StringUtil::Split(url.substr(argStart + 1), "&", args);

			url.erase(argStart + 1);
			for (size_t i = 0; i < args.size(); ++i) {

				if (i > 0)
					url += "&";

				std::string &arg = args[i];

				size_t equal = arg.find_first_of('=');
				if (equal != std::string::npos) {
					std::string key(arg.substr(0, equal));
					std::string value(arg.substr(equal + 1));
					if (!StringUtil::IsEscaped(value)) {
						value = StringUtil::UrlEscape(value);
					}
					arg = key + "=" + value;
				}
				else if (!StringUtil::IsEscaped(arg))
					arg = StringUtil::UrlEscape(arg);

				url += arg;
			}
		}

		Json::Value json;
		json["url"] = url;
		Log::Debug("NaoURLService", "URL = %s", url.c_str());
		//////////////////////////
		try
		{
			// Check wifi status on tablet
			m_Tablet.call<void>("cleanWebview");
			bool webViewTest = m_Tablet.call<bool>("showWebview", url);
			Log::Debug("NaoURLService", "showWebView: %d", webViewTest);

			URLService::URLServiceData urlServiceData;

			if (webViewTest)
			{
				urlServiceData.m_JsonValue = Json::Value("COMPLETED");
				urlServiceData.m_spUrl = a_spUrl;
				m_spTabletDisplayTimer.reset();
				m_spTabletDisplayTimer = TimerPool::Instance()->StartTimer( VOID_DELEGATE(NaoURLService, OnDisplayLogo, this), 
					m_TabletDisplayTime, true, false);
				m_bDisplayLogo = false;
			}
			else
			{
				urlServiceData.m_JsonValue = Json::Value();
				urlServiceData.m_spUrl = a_spUrl;
				ConfigureTablet();
			}

			if (a_Callback.IsValid())
			{
				a_Callback(urlServiceData);
			}
		}
		catch (  const std::exception & ex )
		{
			Log::Debug("NaoURLService", "Received the following error trying to show wayblazer on the tablet: %s", ex.what());
		}
	}
	else
	{
		//base class function call
		URLService::SendURL(a_spUrl, a_Callback ); // --> base child
	}
}

void NaoURLService::OnCheckConnection()
{
	try 
	{
		if (m_Tablet)
		{
			m_bTabletConnected = (m_Tablet.call<std::string>("getWifiStatus") ).compare("CONNECTED") == 0;
			Log::Debug("NaoURLService", "Checking tablet connection - wifi status: %s", m_bTabletConnected ? "Connected" : "Failed" );			
			if ( m_bTabletConnected && m_bDisplayLogo )
			{
				const std::string & logo = SelfInstance::GetInstance()->GetLogoUrl();			
				Log::Debug( "NaoURLService", "Setting logo: %s", logo.c_str() );
				m_bDisplayLogo = ! m_Tablet.call<bool>("showImage", logo);
				if ( m_bDisplayLogo )
					Log::Debug("NaoURLService", "Display logo failed.");
			}	
		}
		else
		{
			m_bTabletConnected = false;
		}
	}
	catch (const std::exception & e)
	{
		Log::Error("NaoURLService", "Threw exception during configure: %s", e.what() );
		m_bTabletConnected = false;		
	}
	if (! m_bTabletConnected)
		ConfigureTablet();
}

void NaoURLService::ConfigureTablet()
{	
	try
	{
		Log::Debug("NaoURLService", "Reconfiguring tablet...");
		if (NaoPlatform::Instance()->HasService("ALTabletService" ))
		{
			m_Tablet = NaoPlatform::Instance()->GetSession()->service("ALTabletService");
			if (m_Tablet)
			{
				// Configure wifi
				m_Tablet.call<void>("resetTablet");			
				m_Tablet.call<void>("enableWifi");
				// m_Tablet.call<bool>("configureWifi", m_Security, m_SSID, m_Password); <-- This will be uncommented when creds can be pulled from gateway
				m_bTabletConnected = ( m_Tablet.call<std::string>("getWifiStatus") ).compare("CONNECTED") == 0;
				// Other config
				Log::Debug("NaoURLService", "Setting brightness to %.2f", m_fBrightness);
				bool set = m_Tablet.call<bool>("setBrightness", m_fBrightness);
				if (! set )
					Log::Debug("NaoURLService", "Failed to set brightness");
			}
		}
	}
	catch (const std::exception & e)
	{
		Log::Error("NaoURLService", "Threw exception during configure: %s", e.what() );
	}
}

qi::AnyReference NaoURLService::OnTouchData(const std::vector <qi::AnyReference> & args)
{
	Log::Debug("NaoURLService", "Touch registered - keeping alive" );
	m_spTabletDisplayTimer.reset();
	m_spTabletDisplayTimer = TimerPool::Instance()->StartTimer( VOID_DELEGATE(NaoURLService, OnDisplayLogo, this), 
		m_TabletDisplayTime, true, false);
	m_bDisplayLogo = false;

	return qi::AnyReference();
}

void NaoURLService::OnDisplayLogo()
{
	Log::Debug("NaoURLService", "Wayblazer timeout - reloading logo");
	m_bDisplayLogo = true;
}
