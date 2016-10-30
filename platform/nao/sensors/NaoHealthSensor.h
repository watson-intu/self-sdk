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

#ifndef NAO_DEVICE_H
#define NAO_DEVICE_H

#include "sensors/HealthData.h"
#include "sensors/HealthSensor.h"

#include "utils/TimerPool.h"
#include "utils/Time.h"

#include "NaoPlatform.h"
#include "SelfInstance.h"
#include "SelfLib.h"

//! NAO device (actuator or sensor) sensor class
class NaoHealthSensor : public HealthSensor
{
public:
    RTTI_DECL();

    NaoHealthSensor() : m_StopThread( false ), m_ThreadStopped( false ), m_RobotFallenCount( 0 ),
                        m_LastPosture( "" ), m_bBatteryCharging( false ), m_bEnableFallRecovery( false ),
                        m_fTimeForPostureTransition( 12.0f ), m_fLowTempThreshold( 20.0f ), m_fHighTempThreshold( 60.0f ),
                        m_fHeadHighTempThreshold( 80.0f ), m_RepairThreshold( 3 ),
                        m_LastCriticalFailure( 0 ), m_CriticalFailureLifespan( 14400 ), m_CriticalFailureBufferTime( 300 ),
                        m_LastRepairAlert( 0 ), m_RepairAlertBufferTime( 300 ),
                        m_LastLowBattery( 0 ), m_LowBatteryBufferTime( 300 ),
                        m_LastAbnormalTemp( 0 ), m_AbnormalTempBufferTime( 600 )
    {}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISensor interface
    virtual const char * GetSensorName()
    {
        return "Health";
    }

    virtual bool OnStart();
    virtual bool OnStop();

protected:
    //! Data
    std::string                 m_DeviceName;

private:
    //! Data
    qi::AnyObject               m_Memory;
	qi::AnyObject               m_BatteryPowerSub;

	NaoPlatform *               pPlatform;
    volatile bool 			    m_StopThread;
    volatile bool               m_ThreadStopped;

	std::vector<std::string>    m_StablePostures;
	std::vector<std::string>    m_ImportantJoints;
	boost::atomic<unsigned int> m_RobotFallenCount;
	std::string                 m_LastPosture;
	Time                        m_LastPostureTime;
	Delegate<const std::string &>
								m_PostureNotifier;
	bool                        m_bBatteryCharging;
	bool                        m_bEnableFallRecovery;

	float                       m_fTimeForPostureTransition;
	float                       m_fLowTempThreshold;
	float                       m_fHighTempThreshold;
	float                       m_fHeadHighTempThreshold;

	double                      m_LastCriticalFailure;
	double                      m_CriticalFailureLifespan;
	double                      m_CriticalFailureBufferTime;

	int                         m_RepairThreshold;
	double                      m_LastRepairAlert;
	double                      m_RepairAlertBufferTime;

	double                      m_LastLowBattery;
	double                      m_LowBatteryBufferTime;

	double                      m_LastAbnormalTemp;
	double                      m_AbnormalTempBufferTime;

	std::map<std::string, double>
								m_FailedSensors;
	std::map<std::string, double>
								m_RecoveredSensors;

	void                        SensorDiagnosis( void * arg );
    void				        CallSensorDiagnosis( void * arg );
    void                        SendHealthData( HealthData * a_pData );
	void                        OnPostureChanged( const std::string & posture );
	void                        FilterRecoveredSensorsMap();

	qi::AnyReference            OnBatteryPowerChanged( const std::vector<qi::AnyReference> & args );

	bool                        IsStablePosture(const std::string & posture);
	bool                        IsRestPosture();
	bool                        IsHeadJoint(const std::string & joint);
	bool                        IsImportantJoint(const std::string & joint);
	std::string                 SimplifyKey(const std::string & key);
	void                        BuildHealthData( const char * name, const char * sensor, const char * state,
	                                             float value, bool error, bool raiseAlert );
};

#endif	// NAO_DEVICE_H