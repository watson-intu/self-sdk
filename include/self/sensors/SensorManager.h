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

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <list>

#include "boost/shared_ptr.hpp"
#include "utils/Factory.h"
#include "utils/Delegate.h"
#include "utils/RTTI.h"
#include "ISensor.h"
#include "SelfLib.h"				// include last

//! Forward declare
class SelfInstance;
class ISensor;
class IData;
class TopicManager;
class ProxySensor;

//! This manager initializes all sensor objects for the local environment. Various systems in Self can access
//! a sensor object and subscribe to receive data from a given sensor when it occurs.
class SELF_API SensorManager 
{
public:
	//! Types
	typedef boost::shared_ptr<ISensor>		ISensorSP;
	typedef std::vector< ISensorSP >		SensorList;
	typedef Delegate<IData *>				SensorDelegate;
	typedef Delegate<ISensor *>				OnSensor;

	//! Construction
	SensorManager();

	//! Accessors
	const SensorList &		GetSensors() const;
	//! Finds a sensor by it's ID
	ISensorSP				FindSensor( const std::string & a_SensorId ) const;
	//! Finds all sensors by the specified data type, returns true if any sensors are found.
	bool					FindSensorsByDataType(const std::string & a_DataType, SensorList & sensors) const;

	//! Initialize and start this sensor manager.
	bool					Start();
	//! Stop this sensor manager.
	bool					Stop();

	//! Add the sensor to this manager, it takes ownership of the object if accepted.
	//! If a_bOverride is true, any sensor of the same data type will be disabled.
	bool					AddSensor( const ISensorSP & a_spSensor, bool a_bOverride = false );
	//! Remove a sensor from this manager.
	bool					RemoveSensor( const ISensorSP & a_pSensor );
	//! Register for sensors for the given data type, the callbacks will be invoked when a sensor is added or removed.
	void					RegisterForSensor( const std::string & a_DataType,
								OnSensor a_OnAddSensor, 
								OnSensor a_OnRemoveSensor );
	//! Unregister for sensor callbacks
	bool					UnregisterForSensor( const std::string & a_DataType,
								void * a_pObject = NULL );

	//! Pause all sensors of the given type
	void					PauseSensorType(const std::string & a_dataType);
	//! Resume all sensors of the given type
	void					ResumeSensorType(const std::string & a_dataType);

	//! Callbacks from ISensor
	void					OnSensorOverride( ISensor * a_pSensor );
	void					OnSensorOverrideEnd( ISensor * a_pSensor );

	template<typename T>
	boost::shared_ptr<T> FindSensorType() const
	{
		for(SensorList::const_iterator iSensor = m_Sensors.begin();
			iSensor != m_Sensors.end(); ++iSensor)
		{
			boost::shared_ptr<T> spSensor = DynamicCast<T>((*iSensor));
			if(spSensor)
				return spSensor;
		}

		return NULL;
	}

	template<typename T>
	bool RemoveSensorType()
	{
		boost::shared_ptr<T> spSensor = FindSensorType<T>();
		if ( spSensor )
			return RemoveSensor( spSensor );

		return false;
	}


	void RemoveSensorsByDataType( const std::string & a_DataType )
	{
		SensorList sensors;
		FindSensorsByDataType( a_DataType, sensors );

		for( SensorList::iterator iSensor = sensors.begin(); iSensor != sensors.end(); ++iSensor )
			RemoveSensor( (*iSensor) );
	}

	//! Deprecated, use FindSensorsByDataType instead.
	bool FindSensors(const RTTI & a_DataType, SensorList & sensors) const
	{
		return FindSensorsByDataType( a_DataType.GetName(), sensors );
	}

	void RemoveSensorsByDataType( const RTTI & a_DataType )
	{
		RemoveSensorsByDataType( a_DataType.GetName() );
	}

private:
	//! Types
	struct SensorRegistry 
	{
		SensorRegistry()
		{}
		SensorRegistry( OnSensor a_OnAdd, OnSensor a_OnRemove ) : m_OnAddSensor( a_OnAdd ), m_OnRemoveSensor( a_OnRemove )
		{}

		OnSensor		m_OnAddSensor;
		OnSensor		m_OnRemoveSensor;
	};
	typedef std::list<SensorRegistry>					RegistryList;
	typedef std::map< std::string, RegistryList>		RegistryMap;

	typedef boost::weak_ptr<ProxySensor>	ProxySensorWP;
	typedef std::list< ProxySensorWP >		ProxyList;

	//! Data
	bool					m_bActive;
	SensorList				m_Sensors;				// list of active sensors
	RegistryMap				m_RegistryMap;
	TopicManager *			m_pTopicManager;

	//! Callbacks
	void					OnSubscriber( const ITopics::SubInfo & a_Info );
	void					OnSensorEvent( const ITopics::Payload & a_Payload );
};

inline const SensorManager::SensorList & SensorManager::GetSensors() const
{
	return m_Sensors;
}

#endif
