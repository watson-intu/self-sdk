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

#ifndef SELF_GRAPH_CONNECTOR_H
#define SELF_GRAPH_CONNECTOR_H

#include <boost/enable_shared_from_this.hpp>

#include "utils/TimerPool.h"
#include "topics/ITopics.h"
#include "IGraph.h"
#include "SelfLib.h"

//! This class is used to connect graphs through the ITopics interface.
class SELF_API GraphConnector : public IGraph::IObserver, 
	public boost::enable_shared_from_this<GraphConnector>
{
public:
	//! Types
	typedef boost::shared_ptr<GraphConnector>	SP;
	typedef boost::weak_ptr<GraphConnector>		WP;
	typedef Delegate<GraphConnector *>			OnGraphConnected;
	typedef unsigned int						EventId;
	typedef IGraph::IEdge						IEdge;
	typedef IGraph::IVertex						IVertex;
	typedef IGraph::ITraverser					ITraverser;

	//! Base class for all events published to the graph-topic
	struct SELF_API IGraphEvent : public ISerializable
	{
		RTTI_DECL();

		//! Types
		typedef Delegate<IGraphEvent *>			EventCallback;
		typedef boost::shared_ptr<IGraphEvent>	SP;

		//! Construction
		IGraphEvent(EventId a_nEventId = 0) : m_nEventId(a_nEventId), m_pConnecor(NULL)
		{}
		virtual ~IGraphEvent()
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IGraphEvent interface
		virtual void Execute() = 0;

		//! Data
		EventId					m_nEventId;

		GraphConnector *		m_pConnecor;
		std::string				m_Origin;
	};

	//! This is sent to traverse a graph in the parent graph, the parent 
	//! will then send a TraverseResult back to the child to merge data into the child's graph 
	//! before it attempts to search.
	struct SELF_API TraverseRequest : public IGraphEvent
	{
		RTTI_DECL();

		TraverseRequest()
		{}
		TraverseRequest(const ITraverser::SP & a_spTraveser) :
			m_spTraverser(a_spTraveser)
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IGraphEvent interface
		virtual void Execute();
		//! Callback for the local traversal
		void OnTraverseComplete(IGraph::ITraverser::SP a_spTraveser);

		//! Data
		ITraverser::SP	m_spTraverser;
	};
	//! This is sent to a child in response to a TraverseRequest
	struct SELF_API TraverseResult : public IGraphEvent
	{
		RTTI_DECL();

		TraverseResult()
		{}
		TraverseResult(EventId a_nEventId, const IGraph::SP & a_spGraph) : m_spGraph(a_spGraph), IGraphEvent(a_nEventId)
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IGraphEvent interface
		virtual void Execute();
		//! Data
		IGraph::SP	m_spGraph;
	};

	//! This is sent to the parent graph when a vertex is created
	struct SELF_API CreateVertEvent : public IGraphEvent
	{
		RTTI_DECL();

		CreateVertEvent()
		{}
		CreateVertEvent(const IVertex::SP & a_spVert) : m_spVert(a_spVert)
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IGraphEvent interface
		virtual void Execute();
		//! Data
		IVertex::SP	m_spVert;
	};

	//! This is sent to the parent when a vertex is removed
	struct SELF_API RemoveVertEvent : public IGraphEvent
	{
		RTTI_DECL();

		RemoveVertEvent()
		{}
		RemoveVertEvent(const IVertex::SP & a_spVert) : m_HashId(a_spVert->GetHashId())
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IGraphEvent interface
		virtual void Execute();
		//! Data
		std::string m_HashId;
	};

	struct SELF_API CreateEdgeEvent : public IGraphEvent
	{
		RTTI_DECL();

		CreateEdgeEvent()
		{}
		CreateEdgeEvent(const IEdge::SP & a_spEdge) :
			m_spEdge(a_spEdge),
			m_SrcHashId(a_spEdge->GetSource()->GetHashId()),
			m_DstHashId(a_spEdge->GetDestination()->GetHashId())
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IGraphEvent interface
		virtual void Execute();
		//! Data
		IEdge::SP m_spEdge;
		std::string m_SrcHashId;
		std::string m_DstHashId;
	};

	struct SELF_API RemoveEdgeEvent : public IGraphEvent
	{
		RTTI_DECL();

		RemoveEdgeEvent()
		{}
		RemoveEdgeEvent(const IEdge::SP & a_spEdge) :
			m_EdgeHashId(a_spEdge->GetHashId()),
			m_SrcHashId(a_spEdge->GetSource()->GetHashId()),
			m_DstHashId(a_spEdge->GetDestination()->GetHashId())
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IGraphEvent interface
		virtual void Execute();
		//! Data
		std::string m_EdgeHashId;
		std::string m_SrcHashId;
		std::string m_DstHashId;
	};

	//! Construction
	GraphConnector(const IGraph::SP & a_spGraph, 
		const std::string & a_Target = "../." );
	~GraphConnector();

	//! IGraph::IObserver interface
	virtual bool OnStartTraverse( const ITraverser::SP & a_spTraverser );
	virtual void OnCreateVertex( const IVertex::SP & a_spVertex );
	virtual void OnRemoveVertex( const IVertex::SP & a_pVertex );
	virtual void OnCreateEdge( const IEdge::SP & a_spEdge );
	virtual void OnRemoveEdge( const IEdge::SP & a_spEdge );

	//! Returns true if this graph is connected to the topics system.
	bool		IsGraphConnected() const;
	const IGraph::SP & 
				GetGraph() const;
	const std::string & 
				GetTarget() const;

	//! Connect this graph to the provide topic system to allow for sharing graph data
	//! through the topic system.
	bool		Connect( ITopics * a_pTopics, 
					OnGraphConnected a_Callback = OnGraphConnected() );
	//! Disconnect from other graphs.
	bool		Disconnect();

protected:
	//! Types
	typedef Delegate<EventId>						EventCallback;

	struct IEventRequest
	{
		typedef boost::shared_ptr<IEventRequest>		SP;

		virtual ~IEventRequest()
		{}

		virtual void OnEventDone() = 0;
		virtual void OnEventTimeout() = 0;
	};
	template<typename T>
	struct EventRequest : public IEventRequest
	{
		EventRequest( GraphConnector * a_pConnector, 
			EventId a_nEventId,
			Delegate<T> a_Callback, 
			T a_CallbackArg, 
			float a_fTimeout ) :
			m_pConnector(a_pConnector),
			m_nEventId( a_nEventId ),
			m_Callback( a_Callback ),
			m_CallbackArg( a_CallbackArg )
		{
			m_pConnector->m_EventMap[ a_nEventId ] = IEventRequest::SP( this );
			if ( TimerPool::Instance() != NULL )
			{
				m_spTimer = TimerPool::Instance()->StartTimer(
					VOID_DELEGATE( EventRequest, OnEventTimeout, this ), a_fTimeout, true, false );
			}
		}

		GraphConnector  *		m_pConnector;
		EventId					m_nEventId;
		Delegate<T>				m_Callback;
		T						m_CallbackArg;
		TimerPool::ITimer::SP	m_spTimer;

		virtual void OnEventDone()
		{
			m_spTimer.reset();
			if ( m_Callback.IsValid() )
				m_Callback( m_CallbackArg );
			m_pConnector->m_EventMap.erase( m_nEventId );
		}
		virtual void OnEventTimeout()
		{
			Log::Warning( "GraphConnector", "Timeout of event %u", m_nEventId );
			OnEventDone();
		}
	};
	typedef std::map<EventId, IEventRequest::SP >			EventMap;

	IGraph::SP		m_spGraph;
	ITopics *		m_pTopics;
	std::string		m_Target;
	OnGraphConnected	m_OnGraphConnected;

	bool			m_bGraphConnected;
	EventMap		m_EventMap;
	bool			m_bParentEvent;			// set to true while processing an graph event from our parent
	std::string		m_GraphTopicId;
	unsigned int	m_nNextEventId;			// ID of next event we send

	void			OnSubscriber(const ITopics::SubInfo & a_Info);
	void			OnGraphEvent(const ITopics::Payload & a_Info);

	//! Send this event to the parent of this graph, if a callback is provided then this event will be registered with the graph.
	bool			Send(const IGraphEvent & a_Event);
	template<typename T>
	EventId			Send(const IGraphEvent & a_Event,
						Delegate<T> a_Callback,
						T a_CallbackArg,
						float a_fTimeoout = 30.0f );

	//! This is invoked to send a reply to a specific child
	bool			Reply(const std::string & a_ChildOrigin,
						const IGraphEvent & a_Event);
	//! This is invoked to send an event to all children
	bool			Reply(const IGraphEvent & a_Event);

	//! Event Handlers
	void			OnTraverseResult( ITraverser::SP a_spTraverser );
};

//---------------------------------

inline bool GraphConnector::IsGraphConnected() const
{
	return m_bGraphConnected;
}

inline const IGraph::SP & GraphConnector::GetGraph() const
{
	return m_spGraph;
}

inline const std::string & GraphConnector::GetTarget() const
{
	return m_Target;
}

template<typename T>
inline GraphConnector::EventId	GraphConnector::Send( const IGraphEvent & a_Event,
	Delegate<T> a_Callback,
	T a_CallbackArg,
	float a_fTimeoout /*= 30.0f*/ )
{
	IGraphEvent & ev = const_cast<IGraphEvent &>( a_Event );
	ev.m_nEventId = m_nNextEventId++;
	if (! Send( ev ) )
		return 0;

	new EventRequest<T>( this, ev.m_nEventId, a_Callback, a_CallbackArg, a_fTimeoout );
	return ev.m_nEventId;
}

#endif // SELF_GRAPH_CONNECTOR_H
