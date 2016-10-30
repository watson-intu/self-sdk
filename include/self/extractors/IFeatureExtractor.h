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

#ifndef IFEATURE_EXTRACTOR_H
#define IFEATURE_EXTRACTOR_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

#include "utils/ISerializable.h"
#include "SelfLib.h"


//! This class manages all active classifier instances. This classifiers subscribe to sensors 
//! and add concepts to the BlackBoard object contained by the SelfInstance.
class SELF_API IFeatureExtractor : public ISerializable, public boost::enable_shared_from_this<IFeatureExtractor>
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<IFeatureExtractor>		SP;
	typedef boost::weak_ptr<IFeatureExtractor>			WP;

	//! Construction
	IFeatureExtractor()
	{}
	virtual ~IFeatureExtractor()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json) {}
	virtual void Deserialize(const Json::Value & json) {}

	//! Interface
	virtual const char * GetName() const = 0;		// returns the textual name of this classifier
	virtual bool OnStart() = 0;						// invoked when this feature extractor is added to the manager
	virtual bool OnStop() = 0;						// invoked when this feature extractor is being removed or shutdown
};

#endif


