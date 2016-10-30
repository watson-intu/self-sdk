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

#include "IAgent.h"
#include "utils/Factory.h"
#include "utils/ThreadPool.h"
#include "blackboard/LearningIntent.h"

#include "SelfLib.h"

//! Forward Declarations
class SelfInstance;

#ifndef SELF_REMINDERAGENT_H
#define SELF_REMINDERAGENT_H

class SELF_API ReminderAgent : public IAgent
{
public:
    RTTI_DECL();

    //! Construction
    ReminderAgent() : m_Delay( 60.0f )
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:

    //! Data
    std::map<std::string, std::string>	m_ScheduleMap;
    float						        m_Delay;
    std::vector<std::string>            m_Sayings;

    TimerPool::ITimer::SP		        m_spScheduleTimer;

    //! Event Handlers
    void OnLearnedIntent(const ThingEvent &a_ThingEvent);
    void OnAddReminder(LearningIntent::SP a_spIntent);
    void OnCheckSchedule();
};

#endif //SELF_REMINDERAGENT_H
