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

#ifndef SELF_REMOTEDEVICE_H
#define SELF_REMOTEDEVICE_H

#include "RemoteDeviceData.h"
#include "utils/IWebClient.h"
#include "utils/ThreadPool.h"
#include "sensors/ISensor.h"
#include "utils/ParamsMap.h"
#include "utils/TimerPool.h"
#include "SelfLib.h"

//! This is the base class for a interacting with external devices
//! For example, weather station, cameras, microphones
class SELF_API RemoteDevice : public ISensor
{
public:
    RTTI_DECL();

    struct Rest : public ISerializable
    {
        RTTI_DECL();

        typedef IWebClient::Headers		Headers;

        Rest()
        {}

        std::string         m_Params;
        std::string         m_URL;
        std::string         m_Type;
        std::string         m_Body;
        Headers             m_Headers;

        virtual void Serialize(Json::Value & json)
        {
            json["m_URL"] = m_URL;
            json["m_Type"] = m_Type;
            json["m_Body"] = m_Body;
            json["m_Params"] = m_Params;

            int index = 0;
            for( Headers::iterator iHeader = m_Headers.begin(); iHeader != m_Headers.end(); ++iHeader )
            {
                json["m_Headers"][index]["key"] = iHeader->first;
                json["m_Headers"][index++]["value"] = iHeader->second;
            }
        }

        virtual void Deserialize(const Json::Value & json)
        {
            // Required Fields
            m_URL = json["m_URL"].asString();
            m_Type = json["m_Type"].asString();

            // Optional Fields
            if( json.isMember( "m_Body" ) )
                m_Body = json["m_Body"].asString();
            if( json.isMember( "m_Params" ) )
                m_Params = json["m_Params"].asString();
            m_Headers.clear();
            if ( json.isMember( "m_Headers" ) )
            {
                const Json::Value & headers = json["m_Headers"];
                for( Json::ValueConstIterator iHeader = headers.begin(); iHeader != headers.end(); ++iHeader )
                    m_Headers[ (*iHeader)["key"].asString() ] = (*iHeader)["value"].asString();
            }
        }
    };

    //! Construction
    RemoteDevice() : m_fPollInterval( 10.0f ), m_bPaused( false )
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "RemoteDevice";
    }
    virtual const char * GetDataType()
    {
        return RemoteDeviceData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

    void SendingData( RemoteDeviceData * a_pData );

private:
	//! Types
	class DeviceRequest
	{
	public:
		DeviceRequest();
		DeviceRequest(const std::vector<RemoteDevice::Rest> & Rests, RemoteDevice * a_pDevice);
		~DeviceRequest();

		void OnState(IWebClient * a_pConnector);
		void OnResponse(IWebClient::RequestData * a_pResponse);
		void MakeRequest();

	private:
		size_t							m_Index;
		RemoteDevice *					m_pDevice;
		std::vector<RemoteDevice::Rest> m_Rests;
		IWebClient::SP					m_spClient;
		ParamsMap						m_Param;
	};

    //!Data
	float					m_fPollInterval;
    std::vector<Rest>       m_Rests;
    TimerPool::ITimer::SP   m_spWaitTimer;
	bool					m_bPaused;

    void                    StreamingThread();
};

#endif
