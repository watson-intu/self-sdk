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

#ifndef SELF_SYSTEM_H
#define SELF_SYSTEM_H

#include "SelfInstance.h"
#include "ISensor.h"
#include "HealthData.h"

#include "utils/TimerPool.h"
#include "utils/Time.h"

#include "SelfLib.h"

//! Base class for a System sensor class
class SELF_API System : public ISensor
{
public:
	RTTI_DECL();

	System( ) : m_SystemCheckInterval( 60 ), m_fFreeMemoryThreshold( 50.0 ),
	            m_cpuUsageCommand( "top -b -n1 | grep 'Cpu(s)' | awk '{print $2 + $4}' " ),
	            m_diskUsageCommand( "df --total -k -h | tail -1 | awk '{print $5}'"),
	            m_freeMemoryCommand( "free | awk 'FNR == 3 {print $4/($3+$4)*100}'" ),
	            m_lastRebootCommand( "last reboot -F | head -1 | awk '{print $5,$6,$7,$8,$9}'" )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return "System";
	}
	virtual const char * GetDataType()
	{
		return HealthData::GetStaticRTTI().GetName().c_str();
	}

	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

protected:
	//! Data
	int                         m_SystemCheckInterval;
	TimerPool::ITimer::SP       m_SystemCheckTimer;
	bool                        m_bProcessing;
	std::string                 m_cpuUsageCommand;
	std::string                 m_diskUsageCommand;
	std::string                 m_freeMemoryCommand;
	std::string                 m_lastRebootCommand;
	float                       m_fFreeMemoryThreshold;

	void                        SendHealthData( HealthData * a_pData );
	void                        OnCheckSystem( );
	void                        DoOnCheckSystem();
	std::string                 ExecuteCommand( const std::string& command );
	void                        BuildHealthData( const char * state, float value, bool error, bool raiseAlert );
};

#endif	// SELF_SYSTEM_H