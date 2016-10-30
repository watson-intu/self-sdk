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

#ifndef SELF_THINKINGAGENT_H
#define SELF_THINKINGAGENT_H

#include "IAgent.h"
#include "blackboard/IThing.h"
#include "blackboard/Status.h"
#include "utils/Factory.h"
#include "utils/TimerPool.h"
#include "sensors/IData.h"

#include "SelfLib.h"

//! Forward Declarations
class SelfInstance;
class Goal;

class SELF_API ThinkingAgent : public IAgent
{
public:
    RTTI_DECL();

    ThinkingAgent() : m_fProcessingTime( 2.0f )
    {
        m_PleaseWaitText.push_back( "ummmm...." );
    }

    //! ISerialize interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    struct ProcessingIThing
    {
        ProcessingIThing(ThinkingAgent * a_pAgent, IThing::SP a_spThing);
        ProcessingIThing(const ProcessingIThing & a_Copy);
        ~ProcessingIThing();

        void OnTimer();

        ThinkingAgent *     m_pAgent;
        IThing::SP          m_spThing;
		Status::SP			m_spStatus;
        TimerPool::ITimer::SP
							m_spTimer;
    };
    typedef std::list< ProcessingIThing > ProcessingThingList;

    //! Data
    float					m_fProcessingTime;
    std::vector<std::string> m_PleaseWaitText;

    TimerPool *             m_pTimerPool;
    ProcessingThingList     m_ProcessingThingList;

    //! Callbacks
    virtual void OnIntent(const ThingEvent & a_ThingEvent);
    virtual void OnStatus(const ThingEvent & a_ThingEvent);
};

#endif //SELF_THINKINGAGENT_H