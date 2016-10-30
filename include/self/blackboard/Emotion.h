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


#ifndef SELF_EMOTION_H
#define SELF_EMOTION_H

#include "IThing.h"

//! This object is placed on the blackboard when we need self to say anything
class SELF_API Emotion : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Emotion>			SP;
	typedef boost::weak_ptr<Emotion>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Construction
	Emotion() : IThing( TT_COGNITIVE )
	{}
	Emotion( const std::string & a_Type ) : IThing( TT_COGNITIVE ), m_Type( a_Type )
	{}

	//! Accessors
	const std::string & GetType() const
	{
		return m_Type;
	}

	void SetType( const std::string & a_Type )
	{
		m_Type = a_Type;
	}

private:
	//! Data
	std::string m_Type;
};

#endif //SELF_EMOTION_H

