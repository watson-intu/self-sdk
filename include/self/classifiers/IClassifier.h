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

#ifndef ICLASSIFIER_H
#define ICLASSIFIER_H

#include "utils/ISerializable.h"
#include "SelfLib.h"				// include last

class ClassifierManager;

//! This class manages all active classifier instances. This classifiers subscribe to sensors 
//! and add concepts to the BlackBoard object contained by the SelfInstance.
class SELF_API IClassifier : public ISerializable
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<IClassifier>			SP;
	typedef boost::weak_ptr<IClassifier>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json) 
	{}
	virtual void Deserialize(const Json::Value & json) 
	{}

	//! Construction
	IClassifier()
	{}
	virtual ~IClassifier()
	{}

	//! Interface
	virtual const char * GetName() const = 0;		// returns the textual name of this classifier
	virtual bool OnStart() = 0;						// invoked when this classifier is added to the manager
	virtual bool OnStop() = 0;						// invoked when this classifier is being removed or shutdown
};

#endif


