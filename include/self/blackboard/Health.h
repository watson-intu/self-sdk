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


#ifndef SELF_HEALTH_H
#define SELF_HEALTH_H

#include "IThing.h"

class SELF_API Health : public IThing
{
public:
    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr<Health>       SP;
    typedef boost::weak_ptr<Health>         WP;

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! Construction
    Health() : IThing( TT_MODEL, 30.0f ), 
		m_bError( false ),
        m_bTriggerGoal( false ),
        m_fHealthValue( 0.0f )
    {}

	Health(const std::string & a_HealthName, const std::string & a_HealthState, const float a_fHealthValue, bool a_bError,
			bool a_bTriggerGoal) :
		IThing( TT_MODEL, a_HealthState, 30.0f ), 
		m_HealthName( a_HealthName ), 
		m_fHealthValue( a_fHealthValue ), 
		m_bError( a_bError ),
	    m_bTriggerGoal( a_bTriggerGoal )
	{}

	Health(const std::string & a_HealthName, bool a_bError, bool a_bTriggerGoal) :
        IThing( TT_MODEL, a_bError ? "DOWN" : "UP", 30.0f ), 
		m_HealthName( a_HealthName ),
		m_bError( a_bError ),
		m_bTriggerGoal( a_bTriggerGoal ),
		m_fHealthValue( 0.0f )
    {}

    //! Accessors
    const std::string & GetHealthName() const
    {
        return m_HealthName;
    }
	const std::string & GetErrorName() const
	{
		return m_ErrorName;
	}
	bool IsError() const
	{
		return m_bError;
	}
	bool IsTriggerGoal() const
	{
		return m_bTriggerGoal;
	}
	float GetHealthValue() const
	{
		return m_fHealthValue;
	}

    void SetHealthName( const std::string & a_HealthName )
    {
        m_HealthName = a_HealthName;
    }
	void SetErrorName( const std::string & a_ErrorName )
	{
		m_ErrorName = a_ErrorName;
	}

private:
    //! Data
    std::string     m_HealthName;
	std::string     m_ErrorName;
    bool            m_bError;
	bool            m_bTriggerGoal;
	float           m_fHealthValue; // e.g. temperature
};

#endif //SELF_HEALTH_H
