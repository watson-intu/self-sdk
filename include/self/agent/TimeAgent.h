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

#ifndef SELF_TIMEAGENT_H
#define SELF_TIMEAGENT_H

#include "IAgent.h"
#include "services/WeatherInsights.h"
#include "blackboard/TimeIntent.h"
#include "SelfLib.h"
#include <boost/date_time/local_time/local_time.hpp>

class SELF_API TimeAgent : public IAgent
{
public:
    RTTI_DECL();

    TimeAgent() : m_TimeZoneDB( "shared/date_time_zonespec.csv" )
    {}

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Data
    WeatherInsights *				m_pWeatherInsights;
    std::string						m_TimeZone;
	std::string						m_TimeZoneDB;
	TimeIntent::SP					m_spActive;
	boost::local_time::tz_database	m_TZdb;

    //! Event Handlers
    void                    OnTimeIntent(const ThingEvent & a_ThingEvent);
    void					OnLocation(const Json::Value & json);
    void                    OnTime(const Json::Value & json);
    void                    OnExecute(std::string & a_TimeZone);

};

#endif //SELF_TIMEAGENT_H
