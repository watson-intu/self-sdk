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

#ifndef SELF_WEATHERINTENT_H
#define SELF_WEATHERINTENT_H

#include "IIntent.h"

class SELF_API WeatherIntent : public IIntent
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<WeatherIntent>		SP;
	typedef boost::weak_ptr<WeatherIntent>			WP;

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IIntent interface
    virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

	const std::string & GetLocation() const
	{
		return m_Location;
	}

private:
	//! Data
	std::string m_Location;
	Json::Value m_Parse;
};

#endif //SELF_WEATHERINTENT_H