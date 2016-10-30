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


#ifndef SELF_PROXIMITY_H
#define SELF_PROXIMITY_H

#include "IThing.h"

//! An image object taken from camera sensor
class SELF_API Proximity : public IThing
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Proximity>			SP;
	typedef boost::weak_ptr<Proximity>				WP;

	//! Construction
    Proximity() : IThing( TT_PERCEPTION ),
		m_Person(false), 
		m_Distance(1.0f), 
		m_DistanceThreshold(0.5f)
    {}

    Proximity( const std::string & a_sType, bool a_bPerson) : 
		IThing( TT_PERCEPTION ),
		m_Distance(1.0f), 
		m_DistanceThreshold(0.5f),
		m_SensorType( a_sType ),
		m_Person( a_bPerson )
    {}

    Proximity(std::string sType, bool p, float a_Distance) : m_DistanceThreshold(0.5f), IThing (TT_PERCEPTION)
    {
        m_SensorType = sType;
        m_Person = p;
        m_Distance = a_Distance;
    }

	//! Accessors
    bool IsPerson() const
    {
        return m_Person;
    }

    bool IsClassified() const
    {
        return m_IsClassified;
    }

    bool CheckPerson()
    {
        if (m_Distance < m_DistanceThreshold)
        {
            m_Person = true;
        }
        else
        {
            m_Person = false;
        }
        m_IsClassified = true;
        return m_Person;
    }

    float GetDistance() const
    {
        return m_Distance;
    }

    void SetDistance( float d )
    {
        m_Distance = d;
    }

    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

private:
    //! Data
    bool		m_Person;
    float		m_Distance;
    bool		m_IsClassified;
    float		m_DistanceThreshold; /// something to test
    std::string m_SensorType;
};

#endif //SELF_PROXIMITY_H
