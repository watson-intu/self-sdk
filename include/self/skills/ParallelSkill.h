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

#ifndef PARALLEL_SKILL_H
#define PARALLEL_SKILL_H

#include <vector>

#include "ISkill.h"

//! This skill is composed of multiple skills running in sequence.
class ParallelSkill : public ISkill
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<ParallelSkill>	SP;
	typedef boost::weak_ptr<ParallelSkill>		WP;
	typedef std::vector< ISkill::SP >			SkillList;

	//! Construction
	ParallelSkill();
	ParallelSkill( const ParallelSkill & a_Copy );
	virtual ~ParallelSkill();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISkill interface
	virtual bool CanUseSkill();
	virtual void UseSkill( Delegate<ISkill *> a_Callback, const ParamsMap & a_Params);
	virtual bool AbortSkill();
	virtual ISkill * Clone();

	void AddSkill( const ISkill::SP & a_pSkill);
	void ClearSkillList();

private:
	//! Data
	SkillList m_Skills;

	//! State Data
	Delegate<ISkill *> m_Callback;
	int m_ActiveSkills;

	//! Callback
	void OnSkillDone( ISkill * a_pSkill );
};

#endif
