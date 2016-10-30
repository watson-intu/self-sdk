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

#ifndef ISENSOR_H
#define ISENSOR_H

#include <string>
#include <map>
#include <stdio.h>

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

#include "topics/ITopics.h"
#include "utils/ISerializable.h"
#include "utils/Delegate.h"
#include "utils/RTTI.h"
#include "utils/StringUtil.h"
#include "SensorManager.h"
#include "IData.h"
#include "SelfLib.h"

class SensorManager;

//! This is the base class for all sensors that can receive some type of input from an external 
//! source. Examples include connected video camera, microphone input, or data through a connected
//! socket.
class SELF_API ISensor : public ISerializable, 
	public boost::enable_shared_from_this<ISensor>
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<ISensor>	SP;

	//! Construction
	ISensor();
	ISensor( const std::string & a_SensorId );
	virtual ~ISensor();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Interface
	virtual const char * GetSensorName() = 0;
	virtual const char * GetDataType() = 0;
	virtual const char * GetBinaryType() { return "application/json"; }

	//! This is invoked when the first subscriber subscribes to this sensor
	virtual bool OnStart() = 0;
	//! This is invoked when the last subscriber un-subscribes.
	virtual bool OnStop() = 0;
	//! This is invoked to pause this sensor.
	virtual void OnPause() = 0;
	//! This is invoked to restart this sensor.
	virtual void OnResume() = 0;

	//! This is invoke by the SensorManager to register any topics provided by this sensor.
	virtual void OnRegisterTopics( ITopics * a_pTopics );
	//! This is invoked to de-register this sensor from the topic system
	virtual void OnUnregisterTopics( ITopics * a_pTopics );

	//! Accessors
	bool IsActive() const 
	{
		return m_Subscribers.size() > 0 || m_TopicSubscribers > 0;
	}
	const std::string & GetSensorId() const
	{
		return m_SensorId;
	}
	bool IsOverridden() const
	{
		return m_Overrides > 0;
	}

	//! Mutators
	void SetSensorManager( SensorManager * a_pManager, bool a_bOverride );
	//! Subscribe to this sensor, any incoming data should be sent to the given delegate. 
	void Subscribe( Delegate<IData *> a_Subscriber );
	//! Un-subscribe from this sensor, you pass the this pointer of the object that the delegate was initialized with..
	bool Unsubscribe( void * obj );

	void AddOverride();
	void RemoveOverride();

protected:

	//! Types
	typedef std::list< Delegate<IData *> > SubcriberList;

	//! Data
	std::string			m_SensorId;
	SubcriberList		m_Subscribers;
	ITopics *			m_pTopics;
	std::string			m_TopicId;
	int					m_TopicSubscribers;
	int					m_Overrides;

	SensorManager *		m_pManager;
	std::vector< SP >	m_Overriden;

	void OnSubscriber( const ITopics::SubInfo & a_Sub );
	//! Send data to all subscribers of this sensor, note this function takes ownership
	//! of the IData object and will delete it once it's done processing with all sensors.
	void SendData( IData * a_Data );
};

#endif
