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

#ifndef SELF_USE_SKILL_H
#define SELF_USE_SKILL_H

#include "blackboard/IThing.h"
#include "skills/ISkill.h"

class SkillInstance;

//! This object is placed on the blackboard when a skill is used.
class SELF_API UsedSkill : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<UsedSkill>		SP;
	typedef boost::weak_ptr<UsedSkill>			WP;

	//! Construction
	UsedSkill() : IThing( TT_COGNITIVE )
	{}
	UsedSkill( const ISkill::SP & a_spSkill, bool a_bFailed ) : 
		IThing( TT_COGNITIVE ),
		m_spSkill( a_spSkill ),
		m_bFailed( a_bFailed )
	{}

	//! ISerializable interface
	virtual void Serialize( Json::Value & json );
	virtual void Deserialize( const Json::Value & json );

	//! Accessors
	const ISkill::SP & GetSkill() const
	{
		return m_spSkill;
	}
	bool IsFailed() const
	{
		return m_bFailed;
	}

protected:
	//! Data
	ISkill::SP		m_spSkill;
	bool			m_bFailed;
};

#endif //SELF_IINTENT_H
