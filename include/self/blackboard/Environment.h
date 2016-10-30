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

#include "IThing.h"

#ifndef SELF_ENVIRONMENT_H
#define SELF_ENVIRONMENT_H

class SELF_API Environment : public IThing
{
public:
    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr<Environment>		    SP;
    typedef boost::weak_ptr<Environment>			WP;

	Environment() : IThing( TT_PERCEPTION )
	{}

    const int & GetCarbonDioxide() const
    {
        return m_CarbonDioxide;
    }
    const int & GetHumidity() const
    {
        return m_Humidity;
    }
    const float & GetTemperature() const
    {
        return m_Temperature;
    }
    const float & GetPressure() const
    {
        return m_Pressure;
    }

    void SetCarbonDioxide( const int & a_CarbonDioxide )
    {
        m_CarbonDioxide = a_CarbonDioxide;
    }
    void SetHumidity( const int & a_Humidity )
    {
        m_Humidity = a_Humidity;
    }
    void SetTemperature( const float & a_Temperature )
    {
        m_Temperature = a_Temperature;
    }
    void SetPressure( const float & a_Pressure )
    {
        m_Pressure = a_Pressure;
    }

	//! Create this object from a JSON response from a RemoteDevice.
	bool Create(const Json::Value & json);

	//! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

private:
    //! Data
    int     m_CarbonDioxide;
    int     m_Humidity;
    float   m_Temperature;
    float   m_Pressure;

};


#endif //SELF_ENVIRONMENT_H
