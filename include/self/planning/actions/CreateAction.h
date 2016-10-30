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

#ifndef SELF_CREATE_ACTION_H
#define SELF_CREATE_ACTION_H

#include "planning/IAction.h"

//! This action creates a new object on the blackboard attached to the provided goal object.
//! This action uses the ParamsMap of the goal to resolve any variables into their final form
//! then we try to deserialize an object from the provided json.
class SELF_API CreateAction : public IAction
{
public:
	RTTI_DECL();

	//! Construction
	CreateAction();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IAction interface
	virtual void Execute(const Goal::SP & a_spGoal, Delegate<const State &> a_Callback);
	virtual IAction * Clone();

	//! Accessors
	bool AttachToRoot() const;
	float GetImportance() const;
	const ParamsMap & GetGoalParams() const;
	bool GetReplaceParams() const;
	const std::string & GetInitialState() const;
	const std::string & GetCompletedState() const;
	const std::string & GetFailedState() const;

	//! Mutators
	void SetAttachToRoot(bool a_bAttachToRoot);
	void SetImportance(float a_fImportance);
	ParamsMap & GetGoalParams();
	void SetReplaceParams(bool a_bReplace);
	void SetInitalState(const std::string & a_InitalState);
	void SetCompletedState(const std::string & a_WaitState);
	void SetFailedState(const std::string & a_FailedState);

private:
	//! Data
	bool			m_bAttachToRoot;	// if true, object is created on the root of the blackboard instead as a child of the goal.
	float			m_fImportance;		// importance of object to create
	ParamsMap		m_GoalParams;		// parameters to merge into the goal when this action is executed.
	bool			m_ReplaceParams;	// if true, then params from this action replace those in the goal
	Json::Value		m_Object;			// data for the object to create, must include "Type_" as the object is created using ISerializable
	std::string		m_InitialState;		// initial state of the object
	std::string		m_CompletedState;	// if set, then this action will wait for the given state before completing
	std::string		m_FailedState;		// if set, then this action will fail if the object enters this state

	//! This is the instance object that actual creates the object and receives the callbacks
	//! for the object state changes.
	class CreateObject
	{
	public:
		CreateObject(CreateAction * a_pAction, Goal::SP a_spGoal, Delegate<const State &> a_Callback );

	private:
		void OnThingEvent(const ThingEvent & a_Event);

		//! Data
		CreateAction *		m_pAction;
		Goal::SP			m_spGoal;
		IThing::SP			m_spObject;
		Delegate<const IAction::State &>
							m_Callback;
	};
};

inline bool CreateAction::AttachToRoot() const
{
	return m_bAttachToRoot;
}

inline float CreateAction::GetImportance() const
{
	return m_fImportance;
}

inline const ParamsMap & CreateAction::GetGoalParams() const
{
	return m_GoalParams;
}

inline bool CreateAction::GetReplaceParams() const
{
	return m_ReplaceParams;
}

inline const std::string & CreateAction::GetInitialState() const
{
	return m_InitialState;
}

inline const std::string & CreateAction::GetCompletedState() const
{
	return m_CompletedState;
}

inline const std::string & CreateAction::GetFailedState() const
{
	return m_FailedState;
}

inline void CreateAction::SetAttachToRoot(bool a_bAttachToRoot)
{
	m_bAttachToRoot = a_bAttachToRoot;
}

inline void CreateAction::SetImportance(float a_fImportance)
{
	m_fImportance = a_fImportance;
}

inline ParamsMap & CreateAction::GetGoalParams()
{
	return m_GoalParams;
}

inline void CreateAction::SetReplaceParams(bool a_bReplace)
{
	m_ReplaceParams = a_bReplace;
}

inline void CreateAction::SetInitalState(const std::string & a_InitalState)
{
	m_InitialState = a_InitalState;
}

inline void CreateAction::SetCompletedState(const std::string & a_WaitState)
{
	m_CompletedState = a_WaitState;
}

inline void CreateAction::SetFailedState(const std::string & a_FailedState)
{
	m_FailedState = a_FailedState;
}

#endif
