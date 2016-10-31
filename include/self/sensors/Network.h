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

#ifndef SELF_NETWORK_H
#define SELF_NETWORK_H

#include "SelfInstance.h"
#include "ISensor.h"
#include "HealthData.h"

#include "utils/TimerPool.h"
#include "utils/Time.h"
#include "utils/IWebClient.h"

#include "SelfLib.h"

//! Base class for a Network sensor class
class SELF_API Network : public ISensor
{
public:
    RTTI_DECL();

    Network( ) : m_NetworkCheckInterval( 20 ), 
		m_fCheckTimeout( 3.0f ),
		m_bNetworkUp( true ), 
		m_bProcessing( false ),
		m_RequestsCompleted( 0 ),
		m_RequestsPending( 0 ),
		m_RequestsFailed( 0 )
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

	class Request
	{
	public:
		Request( const std::string & a_URL, Delegate<bool> a_Callback, float a_fTimeout );

		virtual ~Request()
		{
			assert( m_spClient->GetState() == IWebClient::CLOSED 
				|| m_spClient->GetState() == IWebClient::DISCONNECTED );
		}

		//! HTTP callbacks
		void OnLocalResponse( );
		void OnState( IWebClient * a_pClient );
		void OnResponse( IWebClient::RequestData * a_pResponse );
		void OnTimeout( );

		//! Data
		IWebClient::SP          m_spClient;
		TimerPool::ITimer::SP	m_spTimeoutTimer;
		Delegate<bool>          m_Callback;
		bool                    m_bDelete;
	};

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "Network";
    }
    virtual const char * GetDataType()
    {
        return HealthData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

	//! Accessors
	bool IsNetworkUp() const
	{
		return m_bNetworkUp;
	}

private:
    //! Data
    std::vector<std::string>    m_Addresses;
    std::string                 m_RemoteIP;
    int                         m_NetworkCheckInterval;
	float						m_fCheckTimeout;
    TimerPool::ITimer::SP       m_NetworkCheckTimer;
	bool						m_bNetworkUp;
	bool                        m_bProcessing;
	Time                        m_LastNetworkDown;

	int                         m_RequestsCompleted;
	int                         m_RequestsPending;
	int                         m_RequestsFailed;

	void                        OnNetworkCheck( );
	void                        OnResponse( bool a_bSuccess );
	void                        SendHealthData( HealthData * a_pData );
	void                        BuildHealthData( const char * state, bool error, bool raiseAlert );
};

#endif	// SELF_NETWORK_H