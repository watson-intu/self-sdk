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

#ifndef SELF_WEATHERAGENT_H
#define SELF_WEATHERAGENT_H

#include "IAgent.h"
#include "services/WeatherInsights.h"
#include "SelfLib.h"

class SELF_API WeatherAgent : public IAgent
{
public:
    RTTI_DECL();

	WeatherAgent() : m_bFahrenheit (true)
    {}

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Data
	WeatherInsights *		m_pWeatherInsights;
	bool					m_bFahrenheit;

    //! Event Handlers
    void                    OnAnomaly(const ThingEvent & a_ThingEvent);
    void                    OnWeatherRequest(const ThingEvent & a_ThingEvent);
	void					OnLocation(const Json::Value & json);
    void                    OnCurrentConditionsReceived( const Json::Value & json );
};

#endif //SELF_WEATHERAGENT_H
