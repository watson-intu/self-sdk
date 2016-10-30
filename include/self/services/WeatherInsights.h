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
#ifndef SELF_WEATHERINSIGHTS_H
#define SELF_WEATHERINSIGHTS_H

#include "services/IService.h"
#include "SelfLib.h"

class SELF_API WeatherInsights : public IService
{
public:
    RTTI_DECL();

    //! Types
    typedef Delegate<const Json::Value &>			SendCallback;

    //! Construction
    WeatherInsights();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    void GetCurrentConditions( SendCallback a_Callback );
	void GetCurrentConditions(const std::string & a_Lat, const std::string & a_Long, SendCallback a_Callback);
    void GetHourlyForecast( SendCallback a_Callback );
    void GetTenDayForecast( SendCallback a_Callback );
	void GetLocation( const std::string & a_Location, Delegate<const Json::Value &> a_Callback);
    static void CelsiusToFahrenheit( const float & a_Celsius, float & a_Fahrenheit);

private:

    float m_Latitude;
    float m_Longitude;
    std::string m_Units;
    std::string m_Language;

};


#endif //SELF_WEATHERINSIGHTS_H
