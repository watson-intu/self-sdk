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

#ifndef SELF_PROXY_GESTURE_H
#define SELF_PROXY_GESTURE_H

#include "utils/TimerPool.h"
#include "IGesture.h"
#include "SelfLib.h"

//! This gesture acts as a proxy for a remote gesture to be executed via the topic system
class SELF_API ProxyGesture : public IGesture
{
public:
	RTTI_DECL();

	//! Types
	typedef std::vector< IGesture::SP >		OverrideList;
	typedef boost::shared_ptr<ProxyGesture>	SP;
	typedef boost::weak_ptr<ProxyGesture>	WP;

	//! Construction
	ProxyGesture( const std::string & a_GestureId, 
		const std::string & a_InstanceId,
		bool a_bOverride,
		const std::string & a_Origin );
	ProxyGesture();
	~ProxyGesture();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGesture interface
	virtual bool Start();
	virtual bool Stop();
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap & a_Params);
	virtual bool Abort();

	const std::string & GetInstanceId() const
	{
		return m_InstanceId;
	}
	const std::string & GetOrigin() const
	{
		return m_Origin;
	}

	//! Invoked by the GestureManager when a execute_done event is received.
	void OnExecuteDone( bool a_bError );

private:
	//! Data
	std::string			m_Origin;
	std::string			m_InstanceId;
	bool				m_bOverride;
	OverrideList		m_Overrides;
	TimerPool::ITimer::SP
						m_spTimeoutTimer;

	void ExecuteGesture();
	void OnTimeout();
};

#endif //SELF_WAITGESTURE_H