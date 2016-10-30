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

#ifndef SELF_IACTION_H
#define SELF_IACTION_H

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

#include "utils/ISerializable.h"
#include "utils/Delegate.h"
#include "ICondition.h"

#include "SelfLib.h"

class Plan;
class ICondition;
class PlanInstance;

//! This is the base class for an action that can be attached onto a plan. 
class SELF_API IAction : public ISerializable, public boost::enable_shared_from_this<IAction>
{
public:
	RTTI_DECL();

	//! Types
	enum ActionFlags
	{
		CAN_IGNORE_PRE			= 0x1,	// action can be skipped if pre-conditions are not met
		CAN_IGNORE_POST			= 0x2,	// post-condition failure of this action is ignored and will not cause the plan to fail.
		NON_BLOCKING			= 0x4,	// This action doesn't prevent the next action from executing at the same time
		CAN_IGNORE_ERROR		= 0x8,	// if the action fails, do we ignore the error
	};
	typedef std::vector< ICondition::SP >	Conditions;
	typedef boost::shared_ptr<IAction>		SP;
	typedef boost::weak_ptr<IAction>		WP;

	enum ActionStates
	{
		AS_EXECUTING,			// action is executing
		AS_COMPLETED,			// action is done
		AS_FAILED,				// action failed because of an error
		AS_UNAVAILABLE,			// action was unable to complete
		AS_ABORTED,				// action was aborted by the user
	};

	struct State
	{
		State() : m_pAction(NULL), m_eState(AS_EXECUTING)
		{}

		IAction *		m_pAction;
		ActionStates	m_eState;
	};

	//! Construction
	IAction();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json) ;

	//! This is invoked by the PlanInstance to execute a action of a currently active plan. 
	//! The implementation should invoke the callback when the action is done, success or failure.
	virtual void Execute( const Goal::SP & a_spGoal, Delegate<const State &> a_Callback ) = 0;
	//! Make a new instance copy of this action object.
	virtual IAction * Clone() = 0;

	//! Accessors
	const std::string & GetActionId() const;
	unsigned int GetActionFlags() const;					// returns all the action flags
	bool CheckActionFlag( ActionFlags a_eFlag ) const;		// returns true if the given flag is set
	const Conditions & GetPreConditions() const;			// conditions for this action to execute
	bool TestPreConditions( Goal::SP a_spGoal ) const;
	const Conditions & GetPostConditions() const;			// post conditions expected after this action
	bool TestPostConditions( Goal::SP a_spGoal ) const;

	//! Mutators
	void SetActionFlags( unsigned int a_nFlags );
	void SetPreConditions( const Conditions & a_Conditions );
	void SetPostConditions( const Conditions & a_Conditions );

private:
	//! Data
	std::string			m_ActionId;
	unsigned int		m_ActionFlags;
	Conditions			m_PreConditions;
	float				m_MinPreCondition;
	Conditions			m_PostConditions;
	float				m_MinPostCondition;
};

//------------------------------------------------

inline const std::string & IAction::GetActionId() const
{
	return m_ActionId;
}

inline unsigned int IAction::GetActionFlags() const
{
	return m_ActionFlags;
}

inline bool IAction::CheckActionFlag( ActionFlags a_eFlag ) const
{
	return (m_ActionFlags & a_eFlag) != 0;
}

inline const IAction::Conditions & IAction::GetPreConditions() const
{
	return m_PreConditions;
}

inline bool IAction::TestPreConditions( Goal::SP a_spGoal ) const
{
	float fValue = 1.0f;
	for(size_t i=0;i<m_PreConditions.size();++i)
		fValue *= m_PreConditions[i]->Test( a_spGoal );
	return fValue >= m_MinPreCondition;
}

inline const IAction::Conditions & IAction::GetPostConditions() const
{
	return m_PostConditions;
}

inline bool IAction::TestPostConditions( Goal::SP a_spGoal ) const
{
	float fValue = 1.0f;
	for(size_t i=0;i<m_PreConditions.size();++i)
		fValue *= m_PreConditions[i]->Test( a_spGoal );
	return fValue >= m_MinPreCondition;
}

inline void IAction::SetActionFlags( unsigned int a_nFlags )
{
	m_ActionFlags = a_nFlags;
}

inline void IAction::SetPreConditions( const Conditions & a_Conditions )
{
	m_PreConditions = a_Conditions;
}

inline void IAction::SetPostConditions( const Conditions & a_Conditions )
{
	m_PostConditions = a_Conditions;
}

#endif
