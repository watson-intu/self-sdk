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

#ifndef GESTURE_SKILL_H
#define GESTURE_SKILL_H

#include "gestures/IGesture.h"

#include <list>
#include "ISkill.h"

//! This skill executes a gestures in the gesture manager.
class GestureSkill : public ISkill
{
public:
	RTTI_DECL();

	//! Types
	typedef std::list< ISkill * >		SkillList;

	//! Construction
	GestureSkill();
	GestureSkill( const GestureSkill & a_Copy );
	GestureSkill( const std::string & a_GestureID);

	const std::string & GetGestureId() const
	{
		return m_GestureId;
	}
	const ParamsMap & GetGestureParams() const
	{
		return m_GestureParams;
	}

	void SetGestureId(const std::string & a_GestureId)
	{
		m_GestureId = a_GestureId;
	}
	void SetGestureParams( const ParamsMap & a_Params )
	{
		m_GestureParams = a_Params;
	}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ISkill interface
	virtual bool CanUseSkill();
	virtual void UseSkill( Delegate<ISkill *> a_Callback, const ParamsMap & a_Params);
	virtual bool AbortSkill();
	virtual ISkill * Clone();

private:
	//! Data
	std::string m_GestureId;
	ParamsMap m_GestureParams;		
	bool m_bReplaceParams;
	IGesture::SP m_spGesture;

	void StartGesture();
	void OnGestureCompleted( const IGesture::Result & a_State );
};

#endif //GESTURE_SKILL_H
