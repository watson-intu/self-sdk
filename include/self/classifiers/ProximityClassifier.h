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

#ifndef SELF_PROXIMITY_CLASSIFIER_H
#define SELF_PROXIMITY_CLASSIFIER_H

#include <list>

#include "IClassifier.h"
#include "blackboard/ThingEvent.h"
#include "blackboard/Proximity.h"
#include "SelfLib.h"

class SelfInstance;
class Object;

//! This classifier subscribes to all video sensors and classifies all incoming video data
//! which then adds a Concept to the BlackBoard.
class SELF_API ProximityClassifier : public IClassifier
{
public:
	RTTI_DECL();

	ProximityClassifier()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IClassifier interface
	virtual const char * GetName() const;
	virtual bool OnStart();
	virtual bool OnStop();

private:

	//! Data
	Proximity::SP				m_spProximity;


	//! Callback handler
	void OnProximityClassified(const Json::Value & responseValue);
	void OnProximity(const ThingEvent & a_ThingEvent);
};

#endif

