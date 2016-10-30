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

#ifndef ITOPIC_MANAGER_H
#define ITOPIC_MANAGER_H

#include <list>
#include <vector>

#include "utils/ISerializable.h"
#include "utils/Delegate.h"
#include "utils/RTTI.h"
#include "SelfLib.h"

/*
	This is the base interface class that the self instance needs to be able to communicate with other self instances. 

	These self instances are almost always connected to a parent self to form a hierarchy exactly like a file
	system directory hierarchy. Each self has an ID, which is used to reference topics on that given child. 
	
	A Topic is effectively a path, if for example I wanted to subscribe to the topic (sensors) on my parent from a child
	I would use the topic "../sensors" when I call subscribe. If I wanted to subscribe the same topic on a child called
	self01, my topic would be "self01/sensors". The "+" character acts as wild-card, if for example I wanted to subscribe
	to the self blackboard of all children, I would subscribe to "+/self".
*/

class SELF_API ITopics 
{
public:
	struct SubInfo 
	{
		SubInfo() : m_Subscribed(false)
		{}

		bool				m_Subscribed;	// true if subscribing, false if un-subscribing
		std::string			m_Origin;		// who is the subscriber
		std::string			m_Topic;		// topic they are subscribing too
	};
	typedef Delegate< const SubInfo & >		SubCallback;

	struct Payload
	{
		Payload() : m_Persisted(false)
		{}

		std::string			m_Topic;		// the topic of this payload
		std::string			m_Origin;		// who sent this payload
		std::string			m_Data;			// the payload data
		std::string			m_Type;			// the type of data
		bool				m_Persisted;	// true if this was a persisted payload
		std::string			m_RemoteOrigin;	// this is set to the origin that published this payload 
	};
	typedef Delegate<const Payload &>		PayloadCallback;

	struct TopicInfo 
	{
		TopicInfo()
		{}
		TopicInfo( const std::string & a_TopicId, const std::string & a_TopicType ) : 
			m_TopicId( a_TopicId ), m_Type( a_TopicType )
		{}

		std::string			m_TopicId;		// the ID of this topic
		std::string			m_Type;			// type of topic
	};
	typedef std::vector<TopicInfo>			TopicVector;
	typedef std::vector<std::string>		ChildVector;

	struct QueryInfo
	{
		QueryInfo() : m_bSuccess(false)
		{}

		bool				m_bSuccess;
		std::string			m_Path;
		std::string			m_GroupId;
		std::string			m_SelfId;
		std::string			m_ParentId;
		std::string			m_Name;
		std::string			m_Type;
		ChildVector			m_Children;
		TopicVector			m_Topics;
	};
	typedef Delegate<const QueryInfo &>		QueryCallback;

	//! This returns a list of registered topics.
	virtual void GetTopics( TopicVector & a_Topics ) = 0;
	//! Local systems should register what topics they will be publishing, this allows other
	//! clients to enumerate available topics since some topics are not published unless they have
	//! subscribers.
	virtual void RegisterTopic(
		const std::string & a_TopicId,
		const std::string & a_Type,
		SubCallback a_SubscriberCallback = SubCallback() ) = 0;
	//! unregister a topic
	virtual void UnregisterTopic( const std::string & a_TopicId) = 0;

	//! This will return true if anyone is subscribed to the given topic.
	virtual bool IsSubscribed(const std::string & a_TopicId) = 0;
	//! This returns a list of subscribers for the given topic, the strings returned will be 
	//! relative paths to the given subscribers. 
	virtual size_t GetSubscriberCount(const std::string & a_TopicId) = 0;
	//! Publish data for a given topic to all subscribers.
	virtual bool Publish( 
		const std::string & a_TopicId, 
		const std::string & a_Data, 
		bool a_bPersisted = false,
		bool a_bBinary = false ) = 0;
	//! Send data for a given topic to a specific subscriber
	virtual bool Send(
		const std::string & a_Targets,
		const std::string & a_TopicId,
		const std::string & a_Data,
		bool a_bBinary = false ) = 0;
	//! Publish data for a remote target specified by the provided path.
	virtual bool PublishAt(
		const std::string & a_Path,
		const std::string & a_Data,
		bool a_bPersisted = false,
		bool a_bBinary = false ) = 0;

	//! This queries a node specified by the given path.
	virtual void Query(
		const std::string & a_Path,				//! the path to the node, we will invoke the callback with a QueryInfo structure
		QueryCallback a_Callback ) = 0;
	//! Subscribe to the given topic specified by the provided path.
	virtual bool Subscribe(
		const std::string & a_Path,		//! The topic to subscribe, ".." moves up to a parent self
		PayloadCallback a_Callback) = 0;
	//! Unsubscribe from the given topic
	virtual bool Unsubscribe( 
		const std::string & a_Path,
		void * a_pObject = NULL ) = 0;

	//! Helper function for appending a topic onto a origin
	static std::string GetPath(const std::string & a_Origin, const std::string & a_Topic);
};

#endif
