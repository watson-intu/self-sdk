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

#ifndef IGESTURE_H
#define IGESTURE_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "utils/UniqueID.h"
#include "utils/ISerializable.h"
#include "utils/Delegate.h"
#include "utils/ParamsMap.h"
#include "SelfLib.h"

//! This class wraps a action the self instance can take, this may be moving a joint, locomotion, 
//! or even making a REST call to a server. 
class SELF_API IGesture : public ISerializable, public boost::enable_shared_from_this<IGesture>
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<IGesture>	SP;
	typedef boost::weak_ptr<IGesture>	WP;

	struct Result
	{
		Result() : 
			m_pGesture( NULL ), 
			m_bError( false )
		{}
		Result( IGesture * a_pGesture, bool a_bError = false ) :
			m_pGesture( a_pGesture ),
			m_bError( a_bError )
		{}

		IGesture *	m_pGesture;
		bool		m_bError;
	};
	typedef Delegate<const Result &>		GestureDelegate;


	//! Construction
	IGesture() : m_GestureId( UniqueID().Get() ), m_Overrides( 0 )
	{}
	IGesture( const std::string & a_GestureId )  : m_GestureId( a_GestureId ), m_Overrides( 0 )
	{}
	virtual ~IGesture()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	const std::string & GetGestureId() const
	{
		return m_GestureId;
	}
	bool IsOverridden() const
	{
		return m_Overrides > 0;
	}

	void SetGestureId( const std::string & a_GestureId )
	{
		m_GestureId = a_GestureId;
	}
	void AddOverride()
	{
		assert( m_Overrides >= 0 );
		m_Overrides += 1;
	}
	void RemoveOverride( )
	{
		assert( m_Overrides > 0 );
		m_Overrides -= 1;
	}

	//! Initialize this gesture, if this returns false then GestureManager will not register this gesture.
	virtual bool Start();
	//! This is called when the GestureManager is stopped.
	virtual bool Stop();
	//! This function will be called by a Skill object, this object should invoke the provided
	//! callback when this gesture is done.
	virtual bool Execute( GestureDelegate a_Callback, 
		const ParamsMap & a_Params );
	//! Abort this gesture, if true is returned then abort succeeded and callback will NOT be invoked.
	virtual bool Abort();

protected:
	//! Types
	struct Request
	{
		Request( GestureDelegate a_Callback, const ParamsMap & a_Params )
			: m_Callback( a_Callback ), m_Params( a_Params ), m_bError( false )
		{}

		GestureDelegate m_Callback;
		ParamsMap m_Params;
		bool m_bError;
	};
	typedef std::list<Request *> RequestList;
	typedef std::vector< WP >	OverrideList;

	//! Data
	std::string		m_GestureId;
	int				m_Overrides;
	RequestList		m_Requests;
	
	bool HaveRequests() const
	{
		return ActiveRequest() != NULL;
	}

	Request * ActiveRequest() const
	{
		if ( m_Requests.begin() != m_Requests.end() )
			return m_Requests.front();
		return NULL;
	}

	//! Push a request into the queue, returns true if this is the first active request.
	bool PushRequest( GestureDelegate a_Callback, const ParamsMap & a_Params )
	{
		bool bFirstRequest = m_Requests.begin() == m_Requests.end();
		m_Requests.push_back( new Request( a_Callback, a_Params ) );
		return bFirstRequest;
	}

	//! pops the current active request from the queue, returns true if there are more requests
	bool PopRequest( bool a_bError = false )
	{
		if ( m_Requests.begin() != m_Requests.end() )
		{
			Request * pRequest = m_Requests.front();
			pRequest->m_bError |= a_bError;

			if ( pRequest->m_Callback.IsValid() )
				pRequest->m_Callback( Result( this, pRequest->m_bError ) );

			delete pRequest;
			m_Requests.pop_front();
		}

		return m_Requests.begin() != m_Requests.end();
	}

	void PopAllRequests()
	{
		for( RequestList::iterator iReq = m_Requests.begin(); iReq != m_Requests.end(); ++iReq )
			delete *iReq;
		m_Requests.clear();
	}
};


#endif //IGESTURE_H
