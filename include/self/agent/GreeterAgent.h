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

#ifndef SELF_GREETERAGENT_H
#define SELF_GREETERAGENT_H

#include "IAgent.h"
#include "blackboard/Person.h"
#include "blackboard/Text.h"
#include "utils/Factory.h"
#include "utils/TimerPool.h"

#include "SelfLib.h"


class SELF_API GreeterAgent : public IAgent
{
public:
    RTTI_DECL();

	GreeterAgent() : m_SaySomething(false), m_WaitTime( 30.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

    //! IAgent interface
	virtual bool OnStart();
	virtual bool OnStop();

private:
	//! Types
	struct Greeting : public ISerializable
	{
		RTTI_DECL();

		Greeting()
		{}
		Greeting( const std::string & a_Greeting ) : m_Greeting( a_Greeting )
		{}

		std::string		m_Greeting;
		std::string		m_GenderFilter;
		std::string		m_AgeFilter;

		virtual void Serialize(Json::Value & json)
		{
			json["m_Greeting"] = m_Greeting;
			if ( m_GenderFilter.size() > 0 )
				json["m_GenderFilter"] = m_GenderFilter;
			if ( m_AgeFilter.size() > 0 )
				json["m_AgeFilter"] = m_AgeFilter;
		}
		virtual void Deserialize(const Json::Value & json)
		{
			m_Greeting = json["m_Greeting"].asString();
			if ( json.isMember( "m_GenderFilter" ) )
				m_GenderFilter = json["m_GenderFilter"].asString();
			if ( json.isMember( "m_AgeFilter" ) )
				m_AgeFilter = json["m_AgeFilter"].asString();
		}
	};

	//! Data
    bool        m_SaySomething;
	std::vector<Greeting>
				m_Greetings;
	std::vector<std::string>
				m_NameGreetings;
	float		m_WaitTime;			// amount of time of silence to engage greeting
	TimerPool::ITimer::SP
				m_spWaitTimer;

    void		OnPerson(const ThingEvent &a_ThingEvent);
	void		OnSpeech(const ThingEvent & a_ThingEvent);
	void		OnEnableGreeting();
};

#endif //SELF_GREETERAGENT_H
