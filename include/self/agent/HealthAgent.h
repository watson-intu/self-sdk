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

#ifndef SELF_HEALTHAGENT_H
#define SELF_HEALTHAGENT_H

#include "IAgent.h"
#include "utils/Factory.h"
#include "sensors/HealthData.h"
#include "sensors/SensorManager.h"
#include "SelfLib.h"
#include "services/IService.h"
#include "utils/TimerPool.h"

class SELF_API HealthAgent : public IAgent
{
public:
    RTTI_DECL();

    //! Types
    typedef std::list<IService::SP>	ServiceList;

    //! Construction
    HealthAgent() : m_HealthCheckInterval( 60 ), m_NetworkCongestionThreshold( 10 ),
                    m_bInServiceFailureState( false ), m_bInNetworkFailureState( false ),
                    m_LastTimeoutCount( 0 )
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! Accessors
    const ServiceList & GetServiceList() const;

    //! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
    //! Types
    typedef SensorManager::SensorList   SensorList;

    //! Data
    int     			                m_HealthCheckInterval;
	int                                 m_NetworkCongestionThreshold;
    TimerPool::ITimer::SP               m_HealthCheckTimer;
    std::map<std::string, bool>         m_HealthStatusMap;
	std::vector<std::string>            m_HealthCheckServices;

	// services
	boost::atomic<unsigned int>         m_FailedServices;
	boost::atomic<unsigned int>         m_WorkingServices;
	bool                                m_bInServiceFailureState;
	bool                                m_bInNetworkFailureState;

	// sensors
	SensorList                          m_HealthSensors;
	int                                 m_LastTimeoutCount;

    //! Event Handlers
    void                                OnHealth(const ThingEvent & a_ThingEvent);
    void                                OnHealthSensorData(IData * data);
    void                                OnCheckServiceStatus();
    void                                OnGetServiceStatus(const IService::ServiceStatus & a_Status);

};

#endif //SELF_HEALTHAGENT_H
