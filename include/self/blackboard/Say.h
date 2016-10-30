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


#ifndef SELF_SAY_H
#define SELF_SAY_H

#include "IThing.h"

//! This object is placed on the blackboard when we need self to say anything
class SELF_API Say : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Say>			SP;
	typedef boost::weak_ptr<Say>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Construction
	Say() : IThing( TT_COGNITIVE )
	{}
	Say( const std::string & a_Text ) :
		IThing( TT_COGNITIVE ),
		m_Text( a_Text )
	{}

	//! Accessors
	const std::string & GetText() const 
	{
		return m_Text;
	}

	void SetText( const std::string & a_Text )
	{
		m_Text = a_Text;
	}

private:
	//! Data
	std::string m_Text;
};

#endif //SELF_IMAGE_H
