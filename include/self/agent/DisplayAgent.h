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

#ifndef SELF_DISPLAYAGENT_H
#define SELF_DISPLAYAGENT_H

#include "IAgent.h"
#include "blackboard/Display.h"
#include "SelfLib.h"

class SkillInstance;

class SELF_API DisplayAgent : public IAgent
{
public:
    RTTI_DECL();

    DisplayAgent();

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    typedef std::list<Display::SP>		DisplayList;
    //! Event Handlers
    void                    OnDisplay(const ThingEvent & a_ThingEvent);
    void                    ExecuteDisplay(Display::SP a_pDisplay);
    void                    OnSkillState( SkillInstance * a_pInstance );

    std::string             m_DisplaySkill;
    Display::SP	            m_spActive;
    DisplayList             m_Displays;

};

#endif //SELF_DISPLAYAGENT_H
