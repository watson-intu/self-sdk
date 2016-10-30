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

#ifndef SELF_PROXY_SENSOR_H
#define SELF_PROXY_SENSOR_H

#include "sensors/ISensor.h"
#include "SelfLib.h"

//! This sensor class is used to represent a remote sensor that is running outside this self instance
class SELF_API ProxySensor : public ISensor
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr< ProxySensor >		SP;
	typedef boost::weak_ptr< ProxySensor >			WP;

	//! Construction
	ProxySensor( const std::string & a_SensorId,
		const std::string & a_SensorName, 
		const std::string & a_DataType,
		const std::string & a_BinaryType,
		const std::string & a_Origin );
	ProxySensor();
	~ProxySensor();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISensor interface
	virtual const char * GetSensorName()
	{
		return m_SensorName.c_str();
	}
	virtual const char * GetDataType()
	{
		return m_DataType.c_str();
	}
	virtual const char * GetBinaryType()
	{
		return m_BinaryType.c_str();
	}

	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();
	virtual void OnRegisterTopics( ITopics * a_pTopics );
	virtual void OnUnregisterTopics( ITopics * a_pTopics  );

	const std::string & GetOrigin() const
	{
		return m_Origin;
	}

protected:
	//! Data
	std::string m_SensorName;
	std::string m_DataType;
	std::string m_BinaryType;
	std::string m_Origin;

	std::string m_ProxyTopicId;

	void SendEvent( const std::string & a_EventName );
	void OnSensorData( const ITopics::Payload & a_Payload );
};

#endif
