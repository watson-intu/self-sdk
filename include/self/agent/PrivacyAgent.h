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

#ifndef SELF_PRIVACYAGENT_H
#define SELF_PRIVACYAGENT_H

#include "IAgent.h"
#include "sensors/SensorManager.h"
#include "services/SpeechToText/SpeechToText.h"
#include "SelfLib.h"

class SELF_API PrivacyAgent : public IAgent
{
public:
    RTTI_DECL();

    PrivacyAgent() : m_LogSpeech( false ), m_LogCamera( false ), m_EnableDynamicOptOut( false ), m_bStoreAudio( false),
                     m_MinimumAge( 25 ), m_AgeTimeout(30.0f)
    {}

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:

    //! Types
    typedef SensorManager::SensorList	SensorList;

    //! Data
    SensorList		        m_VideoSensors;
    SpeechToText *		    m_pSpeechToText;
    bool    m_LogSpeech;
    bool    m_LogCamera;
    bool    m_EnableDynamicOptOut;
    bool    m_bStoreAudio;
    int     m_MinimumAge;
    float   m_AgeTimeout;

    TimerPool::ITimer::SP
            m_spAgeTimeout;

    void    OnPerson(const ThingEvent & a_ThingEvent);
    void    OnAgeTimeout();
    void    AddPrivacyThing();
};

#endif //SELF_PRIVACYAGENT_H
