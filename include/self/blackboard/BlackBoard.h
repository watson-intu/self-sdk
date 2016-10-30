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

#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <vector>
#include <map>

#include "ThingEvent.h"
#include "utils/Delegate.h"
#include "topics/ITopics.h"
#include "IThing.h"

#include "SelfLib.h"		// include LAST always

//! This object holds one or more IThing objects. 

//! * IThing are added to this blackboard by the Classifiers & Feature systems.
//! * IThing are weighted and transformed over time. 
//! * Sanjay suggested we use CNN (convolution neural network) to connect and transform the concepts.

//! http://deeplearning.net/tutorial/lenet.html

class TopicManager;

class SELF_API BlackBoard : public ISerializable
{
public:
	RTTI_DECL();

	//! Types
	typedef IThing::ThingList	ThingList;

	//! Construction
	BlackBoard();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors
	const IThing::SP & GetPerceptionRoot() const;
	const IThing::SP & GetAgencyRoot() const;
	const IThing::SP & GetModelsRoot() const;

	IThing::SP FindThing( const std::string & a_GUID ) const;

	//! Initialize this blackboard
	bool Start();
	bool Stop();

	//! Add a concept to this BlackBoard, this concept will automatically be connected to other concepts to produce a Goal in the end.
	//! This object takes ownership of the provided object and will delete the concept once a goal is produced.
	void AddThing( const IThing::SP & a_spThing );

	//! This removes all concepts from the board
	bool RemoveThing( IThing * a_pThing );

	//! Subscribe to any objects of the given type getting added to this Blackboard.
	//! Optionally, the user may subscribe to only certain events.
	void SubscribeToType( const std::string & a_Type, Delegate<const ThingEvent &> a_callback, 
		ThingEventType eventMask = TE_ALL );

	//! Unsubscribe the given type and callback object. If a_pObject is NULL, then all callbacks for the given
	//! type are cleared. Returns false if no match was found.
	bool UnsubscribeFromType( const std::string & a_Type, void * a_pObject = NULL );

	//! Invoke this function to pass a event to all subscribers for a given object type.
	//! This is invoked automatically by AddThing(), RemoveThing(), and when a state is changed
	//! on a IThing object.
	void OnThingEvent( const ThingEvent & thing );

	//! Deprecated Interface
	void SubscribeToType( const RTTI & type, Delegate<const ThingEvent &> a_callback, 
		ThingEventType eventMask = TE_ALL );
	bool UnsubscribeFromType( const RTTI & type, void * a_pObject = NULL );

private:

	//! Types
	struct Subscriber 
	{
		Subscriber() : m_EventMask( TE_ALL )
		{}
		Subscriber( const Subscriber & a_Copy ) 
			: m_Callback( a_Copy.m_Callback ), m_EventMask( a_Copy.m_EventMask )
		{}
		Subscriber( Delegate<const ThingEvent &> a_Callback, ThingEventType a_EventMask ) 
			: m_Callback( a_Callback ), m_EventMask( a_EventMask )
		{}

		Delegate<const ThingEvent &> m_Callback;
		int m_EventMask;
	};

	typedef std::vector< Subscriber >					SubscriberList;
	typedef std::map<std::string, SubscriberList >		SubscriberMap;
	typedef std::map<std::string, IThing::WP>			ThingMap;

	struct RemoteSubscriber
	{
		typedef boost::shared_ptr<RemoteSubscriber>		SP;

		RemoteSubscriber()
		{}
		RemoteSubscriber( BlackBoard * a_pBlackboard, const std::string & a_Origin, const std::string & a_Type, ThingEventType a_EventMask ) :
			m_pBlackboard( a_pBlackboard ), m_Origin( a_Origin ), m_Type( a_Type ), m_EventMask( a_EventMask )
		{
			m_pBlackboard->SubscribeToType( m_Type, 
				DELEGATE( RemoteSubscriber, OnEvent, const ThingEvent &, this ), m_EventMask );
		}
		~RemoteSubscriber()
		{
			m_pBlackboard->UnsubscribeFromType( m_Type, this );
		}

		void OnEvent( const ThingEvent & a_Event );

		BlackBoard * m_pBlackboard;
		std::string m_Origin;
		std::string m_Type;
		ThingEventType m_EventMask;
	};
	typedef std::list< RemoteSubscriber::SP >			RemoteSubscriberList;
	typedef std::map<std::string,RemoteSubscriberList>	RemoteSubscriberMap;

	//! Data
	IThing::SP		m_spPerceptionRoot;
	IThing::SP		m_spCogntiveRoot;
	IThing::SP		m_spModelsRoot;
	SubscriberMap	m_SubscriberMap;
	ThingMap		m_ThingMap;
	RemoteSubscriberMap
					m_RemoteSubscriberMap;
	TopicManager *	m_pTopicManager;

	void OnBlackboardSubscriber( const ITopics::SubInfo & a_Info );
	void OnBlackboardInput( const ITopics::Payload & a_Payload );

	//! Helpers
	void CreateRoot(const std::string & a_Name, ThingCategory a_eCatgory, IThing::SP & a_spRoot);
	void DeserializeRoot(const std::string & a_Name, ThingCategory a_eCategory,
		const Json::Value & a_Json, IThing::SP & a_spRoot);
};

inline const IThing::SP & BlackBoard::GetPerceptionRoot() const
{
	return m_spPerceptionRoot;
}

inline const IThing::SP & BlackBoard::GetAgencyRoot() const
{
	return m_spCogntiveRoot;
}

inline const IThing::SP & BlackBoard::GetModelsRoot() const
{
	return m_spModelsRoot;
}

inline IThing::SP BlackBoard::FindThing( const std::string & a_GUID ) const
{
	ThingMap::const_iterator iThing = m_ThingMap.find( a_GUID );
	if ( iThing != m_ThingMap.end() )
		return iThing->second.lock();
	return IThing::SP();
}

#endif

