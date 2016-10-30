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

#ifndef IMAGE_CLASSIFIER_H
#define IMAGE_CLASSIFIER_H

#include <list>

#include "IClassifier.h"
#include "blackboard/ThingEvent.h"
#include "blackboard/Image.h"
#include "SelfLib.h"

class SelfInstance;
class Object;

//! This classifier subscribes to all video sensors and classifies all incoming video data
//! which then adds a Concept to the BlackBoard.
class SELF_API ImageClassifier : public IClassifier
{
public:
	RTTI_DECL();

	ImageClassifier() : m_bProcessing( false ), m_fRestartTime( 300.0f )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IClassifier interface
	virtual const char * GetName() const;
	virtual bool OnStart();
	virtual bool OnStop();

	void PosRetrainImageClassifier(Image::SP image, std::string target);
	void NegRetrainImageClassifier(Image::SP image, std::string target);

private:

	//! Data
	Image::SP				m_spImage;
	bool					m_bProcessing;
	float                   m_fRestartTime;
	std::string 			m_ClassifierId;
	std::string 			m_ClassifierName;
	TimerPool::ITimer::SP   m_spRestartTimer;

	//! Callback handler
	void OnImageClassified(const Json::Value & responseValue);
	void OnClassifierTrained(const Json::Value & responseValue);
	void OnImage(const ThingEvent & a_ThingEvent);
	void OnHealth(const ThingEvent & a_Event);
	void CallOnStart();
};

#endif

