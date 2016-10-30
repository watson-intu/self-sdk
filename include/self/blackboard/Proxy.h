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


#ifndef SELF_PROXY_H
#define SELF_PROXY_H

#include "BlackBoard.h"
#include "IThing.h"

//! This thing links to another object on the blackboard.
class SELF_API Proxy : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Proxy>			SP;
	typedef boost::weak_ptr<Proxy>				WP;

	//! Construction
	Proxy( ThingCategory a_eType = TT_INVALID ) : IThing( a_eType )
	{}

	Proxy( ThingCategory a_eType, const IThing::SP & a_spThing ) : 
		IThing( a_eType, a_spThing->GetLifeSpan() )
	{
		if ( a_spThing )
			m_TargetGUID = a_spThing->GetGUID();
	}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	const std::string &	GetTargetGUID() const
	{
		return m_TargetGUID;
	}
	IThing::SP GetTarget() 
	{
		if ( m_pBlackBoard != NULL )
			return m_pBlackBoard->FindThing( m_TargetGUID );
		return IThing::SP();
	}

private:
	//! Data
	std::string			m_TargetGUID;
};

#endif //SELF_IMAGE_H
