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

#ifndef LINEAR_SKILL_H
#define LINEAR_SKILL_H

#include <vector>

#include "ISkill.h"

//! This skill is composed of multiple skills running in sequence.
class LinearSkill : public ISkill
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<LinearSkill>		SP;
	typedef boost::weak_ptr<LinearSkill>		WP;
	typedef std::vector< ISkill::SP >			SkillList;

	//! Construction
	LinearSkill();
	LinearSkill( const LinearSkill & a_Copy );
	virtual ~LinearSkill();

	const SkillList & GetSkillList() const 
	{
		return m_Skills;
	}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISkill interface
	virtual bool CanUseSkill();
	virtual void UseSkill( Delegate<ISkill *> a_Callback, const ParamsMap & a_Params);
	virtual bool AbortSkill();
	virtual ISkill * Clone();

	void AddSkill( const ISkill::SP & a_pSkill );
	void ClearSkillList();

private:
	//! Data
	SkillList m_Skills;

	//! State Data
	Delegate<ISkill *> m_Callback;
	ParamsMap m_Params;
	int m_NextSkill;

	//! Callback
	void OnSkillDone( ISkill * a_pSkill );
};

#endif
