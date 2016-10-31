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

#ifndef SELF_GRAPH_H
#define SELF_GRAPH_H

#include <map>
#include <set>

#include "utils/ISerializable.h"
#include "models/IGraph.h"
#include "topics/ITopics.h"
#include "utils/UniqueID.h"
#include "utils/TimerPool.h"
#include "SelfLib.h"

class IDataStore;

//! This class wraps the graph implementation that is used by self.
class SELF_API GraphSelf : public IGraph
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<GraphSelf>			SP;
	typedef boost::weak_ptr<GraphSelf>				WP;
	typedef unsigned int							EventId;

	class SELF_API Vertex : public IVertex
	{
	public:
		RTTI_DECL();

		typedef boost::shared_ptr<Vertex>		SP;
		typedef boost::weak_ptr<Vertex>			WP;

		Vertex()
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
		//! IVertex interface
		virtual void UpdateHashId();
	};
	class SELF_API Edge : public IEdge
	{
	public:
		RTTI_DECL();

		typedef boost::shared_ptr<Edge>			SP;
		typedef boost::weak_ptr<Edge>			WP;

		Edge()
		{}

		//! ISerializable interface
		virtual void Serialize(Json::Value & json);
		virtual void Deserialize(const Json::Value & json);
	};


	//! base class for all GraphSelf traverser classes
	class SELF_API ISelfTraverser : public IGraph::ITraverser
	{
	public:
		RTTI_DECL();

		typedef IGraph::Condition			Condition;

		ISelfTraverser()
		{}
		ISelfTraverser(const IGraph::Condition & a_Condition, SP a_spNext = SP())
			: ITraverser(a_Condition, a_spNext)
		{}

		//! ITraverser interface
		virtual IGraph::ITraverser::SP Filter(const Condition & a_spCond);
		virtual IGraph::ITraverser::SP Out(const Condition & a_spCond /*= NULL_CONDITION*/);
		virtual IGraph::ITraverser::SP In(const Condition & a_spCond /*= NULL_CONDITION*/);
		virtual IGraph::SP CreateGraph(const std::string & a_GraphId);
		virtual bool OnStartTraverse();

		void OnTraverseResponse(EventId a_nEventId);
	};

	class SELF_API FilterTraverser : public ISelfTraverser
	{
	public:
		RTTI_DECL();

		FilterTraverser()
		{}
		FilterTraverser(const IGraph::Condition & a_Condition, SP a_spNext = SP())
			: ISelfTraverser(a_Condition, a_spNext)
		{}

		//! ITraverser interface
		virtual void OnTraverse();
	};
	class SELF_API OutTraverser : public ISelfTraverser
	{
	public:
		RTTI_DECL();

		OutTraverser()
		{}
		OutTraverser(const IGraph::Condition & a_Condition, SP a_spNext = SP())
			: ISelfTraverser(a_Condition, a_spNext)
		{}

		//! ITraverser interface
		virtual void OnTraverse();
	};
	class SELF_API InTraverser : public ISelfTraverser
	{
	public:
		RTTI_DECL();

		InTraverser()
		{}
		InTraverser(const IGraph::Condition & a_Condition, SP a_spNext = SP())
			: ISelfTraverser(a_Condition, a_spNext)
		{}

		//! ITraverser interface
		virtual void OnTraverse();
	};


	//! Construction
	GraphSelf();
	GraphSelf(const std::string & a_GraphId);
	~GraphSelf();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IGraph interface
	virtual bool Import(const std::string & a_File);
	virtual bool Export(const std::string & a_File);
	virtual bool Load(OnGraphLoaded a_OnGraphLoaded = OnGraphLoaded());
	virtual bool Save();
	virtual bool Close();
	virtual void Clear();
	virtual void Optimize();

	virtual size_t GetEdgeCount() const;
	virtual IEdge::SP GetEdgeByIndex(VertexIndex a_nIndex) const;
	virtual size_t GetVertexCount() const;
	virtual IVertex::SP GetVertexByIndex(VertexIndex a_nIndex) const;
	virtual void MergeGraph(const IGraph::SP & a_spGraph);
	virtual ITraverser::SP CreateTraverser(const Condition & a_spCond = NULL_CONDITION);
	virtual IEdge::SP FindEdge(const IVertex::SP & a_Src,
		const IVertex::SP & a_Dest,
		const HashId & a_HashId);
	virtual IEdge::SP GetEdge(const IVertex::SP & a_Src,
		const IVertex::SP & a_Dest,
		const Json::Value & a_Properties = Json::Value(),
		float a_fWeight = 1.0f);
	virtual bool RemoveEdge(const IEdge::SP & a_spEdge);
	virtual IVertex::SP FindVertex(const std::string & a_HashId);
	virtual IVertex::SP GetVertex(const Json::Value & a_Properties = Json::Value());
	virtual bool AddVertex(const IVertex::SP & a_spVertex);
	virtual bool RemoveVertex(const IVertex::SP & a_spVertex);

	virtual std::string ToString();

private:
	//! Types
	typedef std::map<std::string, IVertex::WP>		VertexHash;
	typedef std::list<VertexIndex>					FreeList;

	//! Data
	UniqueID		m_InstanceId;			// unique ID for this instance of a graph
	VertexHash		m_VertexHash;			// hash of IVertex::WP by their hash ID
	VertexList		m_Verts;
	FreeList		m_FreeVertList;			// list of empty vertex indexes available for re-use
	EdgeList		m_Edges;
	FreeList		m_FreeEdgeList;			// list of empty edge indexes

	IDataStore *	m_pStorage;
	OnGraphLoaded	m_OnGraphLoaded;
	size_t			m_nPendingOps;
	Json::Value		m_GraphRoot;

	TimerPool::ITimer::SP
					m_spSaveTimer;

	void			OnLoadGraph(Json::Value * a_Json);
	void			OnVertexLoaded(Json::Value * a_Json);
	void			OnEdgeLoaded(Json::Value * a_Json);
	void			OnLoadDone();
	void			OnAutoSave();
};

#endif // SELF_IGRAPH_H

