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


#ifndef SELF_CONFIRM_H
#define SELF_CONFIRM_H

#include "IThing.h"

//! This object is placed on the blackboard when we need self to confirm an action
//! before we act. The FeedbackAgent will handle the positive/negative feedback that
//! will trigger the confirm or cancel responses of this object. 
class SELF_API Confirm : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Confirm>			SP;
	typedef boost::weak_ptr<Confirm>			WP;
	typedef Delegate<SP>						ConfirmCallback;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Construction
	Confirm() :  IThing( TT_PERCEPTION ), m_bConfirmed( false )
	{}
	Confirm( const Json::Value & a_Info, ConfirmCallback a_Callback ) :
		IThing( TT_PERCEPTION ),
		m_Info(a_Info),
		m_Callback(a_Callback),
		m_bConfirmed( false )
	{}

	bool IsConfirmed() const 
	{
		return m_bConfirmed;
	}
	const Json::Value & GetInfo() const
	{
		return m_Info;
	}

	void OnConfirmed();
	void OnCancelled();
	
private:
	//! Data
	Json::Value m_Info;
	ConfirmCallback m_Callback;
	bool m_bConfirmed;
};

#endif //SELF_CONFIRM_H
