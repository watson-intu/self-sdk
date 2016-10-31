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

#ifndef SELF_ASIMOVAGENT_H
#define SELF_ASIMOVAGENT_H

#include "IAgent.h"
#include "blackboard/Text.h"

class SkillInstance;

class SELF_API AsimovAgent : public IAgent
{
public:
    RTTI_DECL();

    AsimovAgent()
    {}

    //! ISerializable interface
    void Serialize( Json::Value & json );
    void Deserialize( const Json::Value & json );

    //! IAgent interface
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Event Handlers
    void OnText(const ThingEvent & a_ThingEvent);

    //! Data
    std::vector<std::string>    m_Interruptions;
};

#endif //SELF_ASIMOVAGENT_H
