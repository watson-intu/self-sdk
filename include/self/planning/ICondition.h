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

#ifndef SELF_ICONDITION_H
#define SELF_ICONDITION_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"
#include "jsoncpp/json/json.h"

#include "utils/ISerializable.h"
#include "utils/Logic.h"
#include "blackboard/Goal.h"
#include "SelfLib.h"

//! This is the base class for a condition object that can be attached to a plan as a post/pre condition.
class SELF_API ICondition : public ISerializable, public boost::enable_shared_from_this<ICondition>, public Logic
{
public:
	RTTI_DECL();

	//! Types 
	typedef boost::shared_ptr<ICondition>		SP;
	typedef boost::weak_ptr<ICondition>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Test this condition, returns a value between 0.0 (false) and 1.0 (true). >= 0.5f is consider 
	//! a positive when a binary test is needed.
	virtual float Test( Goal::SP a_spGoal ) = 0;
	//! Make a new instance of this ICondition object.
	virtual ICondition * Clone() = 0;
};

#endif
