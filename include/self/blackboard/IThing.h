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

#ifndef SELF_ITHING_H
#define SELF_ITHING_H

#include <string>
#include <vector>
#include <stdio.h>

#include "boost/enable_shared_from_this.hpp"
#include "boost/shared_ptr.hpp"

#include "utils/UniqueID.h"
#include "utils/Time.h"
#include "utils/ISerializable.h"
#include "utils/RTTI.h"
#include "utils/ParamsMap.h"
#include "utils/Delegate.h"
#include "utils/TimerPool.h"

#include "SelfLib.h"		// include last always

class BlackBoard;
class ThingEvent;
class Proxy;

//! These enums are setup as bit flags so the subscribe can subscribe for specific events.
enum ThingEventType
{
	TE_NONE			= 0x0,			// no flags
	TE_ADDED		= 0x1,			// IThing has been added
	TE_REMOVED		= 0x2,			// IThing has been removed
	TE_STATE		= 0x4,			// state of IThing has changed.
	TE_IMPORTANCE	= 0x8,			// Importance of IThing has changed.

	TE_ALL = TE_ADDED | TE_REMOVED | TE_STATE | TE_IMPORTANCE,
    TE_ADDED_OR_STATE = TE_ADDED | TE_STATE
};

//! All things fall into one of the following categories, things of different types will not 
//! be directly attached to each other but a proxy will be created and connected via GUID
enum ThingCategory
{
	TT_INVALID = -1,
	TT_PERCEPTION,
	TT_COGNITIVE,
	TT_MODEL
};

//! This abstract interface is the base class for any object that can be added
//! to the blackboard. The idea is that things connect to each other automatically
//! using a trained graph, then drive the goals of the system. 
//! These things should automatically remove them-selves after they are no longer needed.

class SELF_API IThing : public ISerializable, public boost::enable_shared_from_this<IThing>
{
public:
	RTTI_DECL();

	static const std::string EMPTY_STRING;

	//! Types
	typedef boost::shared_ptr<IThing>	SP;
	typedef boost::weak_ptr<IThing>		WP;
	typedef std::vector< SP >			ThingList;

	//! Construction
	IThing( ThingCategory a_eCategory, 
		const std::string & a_State = "ADDED",
		float a_LifeSpan = 3600.0f,
		const std::string & a_DataType = EMPTY_STRING, 
		const Json::Value & a_Data = Json::Value::nullRef,
		float a_fImportance = 1.0f ) :
		m_pBlackBoard( NULL ),
		m_pParent( NULL ),
		m_eCategory( a_eCategory ),
		m_DataType( a_DataType ),
		m_Data( a_Data ),
		m_State(a_State),
		m_fImportance( a_fImportance ),
		m_GUID( UniqueID().Get() ),
		m_fLifeSpan(a_LifeSpan)
	{}
	IThing( ThingCategory a_eCategory, 
		float a_LifeSpan ) :
		m_pBlackBoard( NULL ),
		m_pParent( NULL ),
		m_eCategory( a_eCategory ),
		m_State("ADDED"),
		m_fImportance( 1.0f ),
		m_GUID( UniqueID().Get() ),
		m_fLifeSpan(a_LifeSpan)
	{}
	IThing() : 
		m_pBlackBoard( NULL ), 
		m_pParent( NULL ),
		m_eCategory(TT_INVALID),
		m_fImportance( 1.0f ),
		m_GUID( UniqueID().Get() ),
		m_fLifeSpan(0.0f)
	{}
	virtual ~IThing()
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Get our owning blackboard object.
	BlackBoard * GetBlackBoard() const
	{
		return m_pBlackBoard;
	}
	//! Get the category for this object
	ThingCategory GetCategory() const
	{
		return m_eCategory;
	}
	//! Get the unique ID for this object.
	const std::string & GetGUID() const
	{
		return m_GUID;
	}
	//! Get the creation time for this thing
	double GetCreateTime() const
	{
		return m_CreateTime.GetEpochTime();
	}
	//! Age of this thing in seconds
	double GetAge() const
	{
		return Time().GetEpochTime() - m_CreateTime.GetEpochTime();
	}
	float GetLifeSpan() const
	{
		return m_fLifeSpan;
	}

	//! The importance of this thing
	float GetImportance() const
	{
		return m_fImportance;
	}

	const std::string & GetState() const
	{
		return m_State;
	}

	//! Returns our parent object.
	SP GetParent() const
	{
		if ( m_pParent != NULL )
			return m_pParent->shared_from_this();
		return SP();
	}

	//! Other things this thing is related too
	const ThingList & GetChildren() const
	{
		return m_Children;
	}

	//! Returns true if this object is a parent of this object.
	bool IsParent( const IThing::SP & a_spThing ) const
	{
		SP spParent = GetParent();
		while( spParent )
		{
			if ( spParent == a_spThing )
				return true;
			spParent = spParent->GetParent();
		}
		return false;
	}

	//! This finds a parent object of the given type.
	template<typename T>
	boost::shared_ptr<T> FindParentType() const 
	{
		SP spParent = GetParent();
		while (spParent)
		{
			boost::shared_ptr<T> spCasted = DynamicCast<T>(spParent);
			if (spCasted)
				return spCasted;

			spParent = spParent->GetParent();
		}

		return boost::shared_ptr<T>();
	}

	//! Find all children of the given type
	template<typename T>
	bool FindChildrenType( std::vector< boost::shared_ptr<T> > & a_Children, bool a_bRecursive = true ) const
	{
		for(size_t i=0;i<m_Children.size();++i)
		{
			const SP & spChild = m_Children[i];
			if ( a_bRecursive )
				spChild->FindChildrenType<T>( a_Children, true );

			boost::shared_ptr<T> spCasted = DynamicCast<T>( spChild );
			if ( spCasted )
				a_Children.push_back( spCasted );
		}

		return a_Children.size() > 0;
	}

	//! Get the object type
	const std::string & GetDataType() const
	{
		return m_DataType;
	}
	const Json::Value & GetData() const
	{
		return m_Data;
	}

	//! Mutators
	void SetBlackBoard(BlackBoard * a_pBlackBoard);

	void SetCategory( ThingCategory a_eCategory )
	{
		m_eCategory = a_eCategory;
	}
	void SetGUID(const std::string & a_GUID)
	{
		m_GUID = a_GUID;
	}

	void SetImportance(float a_fImportance)
	{
		if ( a_fImportance != m_fImportance )
		{
			m_fImportance = a_fImportance;
			OnImportanceChanged();
		}
	}

	void SetState( const std::string & a_NewState )
	{
		if ( m_State != a_NewState )
		{
			m_State = a_NewState;
			OnStateChanged();
		}
	}

	void AddChild(const SP & a_spThing);
	bool RemoveChild(size_t index);
	bool RemoveChild(IThing * a_pThing);
	void RemoveAllChildren();
	bool RemoveThis();

	//! subscribe to this specific instance for notifications.
	void Subscribe(Delegate<const ThingEvent &> a_Subscriber);
	bool Unsubscribe(void * a_pObject);

	//! Sends the given event to all subscribers and the owning blackboard.
	void SendEvent(const ThingEvent & a_Event);

	void SetDataType( const std::string & a_DataType )
	{
		m_DataType = a_DataType;
	}
	void SetData( const Json::Value & a_Data )
	{
		m_Data = a_Data;
	}
	Json::Value & GetData() 
	{
		return m_Data;
	}

	//! Operator[] for accessing contained data
	Json::Value & operator[]( const std::string & a_Key )
	{
		return m_Data[ a_Key ];
	}
	const Json::Value & operator[]( const std::string & a_Key ) const
	{
		return m_Data[ a_Key ];
	}

	//! Interface
	virtual void OnAttached();				// this is invoked AFTER we have been attached into the hierarchy
	virtual void OnDetach();				// this is invoked BEOFRE we are detached from the hierarchy.
	virtual void OnImportanceChanged();		// invoked when the importance is changed
	virtual void OnStateChanged();			// invoked when the state is changed
	virtual void OnLifeSpanExpired();		// invoked by the timer when the lifespan of this thing is up

	//! Sort the things by time, ascending order places the newest thing last in the last.
	static void SortChron( ThingList & a_Things, bool a_bAcending = true );
	//! Returns a text description of the event type.
	static const char * ThingEventTypeText( ThingEventType a_eType );

protected:

	//! Types
	typedef Delegate<const ThingEvent &>	Subscriber;
	typedef std::list< Subscriber >			SubscriberList;

	//! Data
	BlackBoard *		m_pBlackBoard;		// blackboard containing this thing
	ThingCategory		m_eCategory;		// category of blackboard object
	std::string			m_GUID;				// global unique ID for this thing
	Time				m_CreateTime;		// when was this thing made in seconds since epoch
	float				m_fImportance;		// how important is this thing
	std::string			m_State;			// state of this object
	IThing *			m_pParent;			// our parent thing
	ThingList			m_Children;			// things created from this thing

	std::string			m_DataType;			// type of data contained by this object
	Json::Value			m_Data;				// data of this object

	float				m_fLifeSpan;
	TimerPool::ITimer::SP
						m_spDetachTimer;		// timer that will remove this object after it's life is over
	SubscriberList		m_Subscribers;		// subscribers for this instance
};

//-----------------------------------------------

class ThingEvent
{
public:
	ThingEvent(IThing::SP a_pThing, ThingEventType a_Event) : m_spThing(a_pThing), m_Event(a_Event)
	{}
	ThingEvent() : m_Event(TE_ADDED)
	{}

	ThingEventType GetThingEventType() const
	{
		return m_Event;
	}

	IThing::SP GetIThing() const
	{
		return m_spThing;
	}

private:
	IThing::SP		m_spThing;
	ThingEventType 	m_Event;
};

#endif
