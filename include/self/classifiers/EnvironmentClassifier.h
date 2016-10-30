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

#ifndef SELF_ENVIRONMENTCLASSIFIER_H
#define SELF_ENVIRONMENTCLASSIFIER_H

#include "blackboard/Environment.h"
#include "blackboard/BlackBoard.h"
#include "SelfInstance.h"
#include "utils/Factory.h"
#include "SelfLib.h"
#include "IClassifier.h"

class SELF_API EnvironmentClassifier : public IClassifier
{
public:
    RTTI_DECL();

    EnvironmentClassifier() : m_EnvironmentAnomalyThreshold(0)
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value &json);
    virtual void Deserialize(const Json::Value &json);

    //! IClassifier interface
    virtual const char *GetName() const;
    virtual bool OnStart();
    virtual bool OnStop();

private:
	//! Types
	typedef std::list<Environment::SP>		Environments;

	//! Data
    Environment::SP spEnvironment;
    Environments    m_Environments;
    int             m_EnvironmentAnomalyThreshold;

    //! Callback handler
    void OnEnvironment(const ThingEvent &a_ThingEvent);
};

#endif
