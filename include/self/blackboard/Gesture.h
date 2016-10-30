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


#ifndef SELF_GESTURE_H
#define SELF_GESTURE_H

#include "IThing.h"

//! This object is placed on the blackboard when we need self to say anything
class SELF_API Gesture : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Gesture>			SP;
	typedef boost::weak_ptr<Gesture>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Construction
	Gesture() : IThing(TT_PERCEPTION, 30.0f)
	{}
	Gesture(const std::string & a_Type) : m_Type(a_Type), IThing(TT_PERCEPTION, 30.0f)
	{}

	//! Accessors
	const std::string & GetType() const
	{
		return m_Type;
	}

	void SetType(const std::string & a_Type)
	{
		m_Type = a_Type;
	}

private:
	//! Data
	std::string m_Type;
};

#endif //SELF_GESTURE_H

