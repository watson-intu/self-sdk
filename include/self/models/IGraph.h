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

#ifndef SELF_IGRAPH_H
#define SELF_IGRAPH_H

#include <boost/enable_shared_from_this.hpp>
#include <list>

#include "topics/ITopics.h"
#include "utils/Delegate.h"
#include "utils/ISerializable.h"
#include "utils/IConditional.h"
#include "SelfLib.h"

//! This class wraps the graph implementation that is used by self.
class SELF_API IGraph : public ISerializable, 
	public boost::enable_shared_from_this<IGraph>
{
public:
	RTTI_DECL();

	//! Create a IGraph
	static IGraph * Create( const std::string & a_GraphId );

	//! Forward declare
	class IEdge;

	//! Types
	typedef std::string									HashId;
	typedef boost::shared_ptr<IGraph>					SP;
	typedef boost::weak_ptr<IGraph>						WP;

	typedef Delegate<IGraph::SP>						OnGraphConnected;
	typedef Delegate<IGraph::SP>						OnGraphLoaded;
	typedef int											VertexIndex;
	typedef IConditional								Condition;

	typedef boost::shared_ptr<IEdge>					EdgeSP;
	typedef std::vector< EdgeSP >						EdgeList;

	//! Constants
	static const NullCondition NULL_CONDITION;

	//! This class defines a vertices's in the knowledge graph. 
	class SELF_API IVertex : public ISerializable, 
		public boost::enable_shared_from_this<IVertex>
	{
	public:
		RTTI_DECL();

		//!Types
		typedef boost::shared_ptr<IVertex>			SP;
		typedef boost::weak_ptr<IVertex>			WP;
		typedef boost::shared_ptr<IEdge>			EdgeSP;

		IVertex() : m_pGraph( NULL ), m_nIndex(-1), m_fTime( 0.0 ), m_bDirty( false )
		{}
		virtual ~IVertex()
		{}

		//! Accessors
		IGraph * GetGraph() const;
		VertexIndex GetIndex() const;
		const HashId & GetHashId() const;
		const Json::Value & operator[](const std::string & a_Property) const;
		const Json::Value & GetProperties() const;
		double GetTime() const;
		const EdgeList & GetInEdges() const;
		const EdgeList & GetOutEdges() const;
		bool IsDirty() const;

		//! Mutators
		void SetGraph(IGraph * a_pGraph);
		void SetIndex(VertexIndex a_nIndex);
		virtual void UpdateHashId();

		//! NOTE: You should 
		Json::Value & operator[](const std::string & a_Property);
		Json::Value & GetProperties();
		void SetProperties(const Json::Value & a_Properties, bool a_bUpdateHash = true );
		void SetHashId(const std::string & a_HashId);
		void SetTime(double a_fTime);

		void AddInEdge(const EdgeSP & a_spEdge);
		bool RemoveInEdge(const EdgeSP & a_spEdge);
		void AddOutEdge(const EdgeSP & a_spEdge);
		bool RemoveOutEdge(const EdgeSP & a_spEdge);
		void SetDirty( bool a_bDirty );

	protected:
		//! Data
		IGraph *			m_pGraph;			// the graph this vertex belongs
		VertexIndex			m_nIndex;			// index of this vertex in the graph
		HashId				m_HashId;			// hash of all properties in this vertex (note this is used to merge vertices)
		Json::Value			m_Properties;		// properties of this vertex
		double				m_fTime;			// last time this vertex was touched
		EdgeList			m_InEdges;
		EdgeList			m_OutEdges;
		bool				m_bDirty;			// flag set to true if data has been modified
	};
	typedef std::vector< IVertex::SP >		VertexList;

	//! This object defines a relationship between two vertices in this graph, this is one direction.
	class SELF_API IEdge : public ISerializable, 
		public boost::enable_shared_from_this<IEdge>
	{
	public:
		RTTI_DECL();

		//!Types
		typedef boost::shared_ptr<IEdge>			SP;
		typedef boost::weak_ptr<IEdge>				WP;

		IEdge() : m_fWeight(1.0f), m_nIndex( -1 ), m_fTime( 0.0 ), m_bDirty( false )
		{}
		virtual ~IEdge()
		{}

		//! Accessors
		VertexIndex	GetIndex() const;
		const std::string & GetHashId() const;
		const Json::Value & operator[](const std::string & a_Property) const;
		const Json::Value & GetProperties() const;
		float GetWeight() const;
		double GetTime() const;
		IVertex::SP GetSource() const;
		IVertex::SP GetDestination() const;
		bool IsDirty() const;

		//! Mutators
		void SetIndex(VertexIndex a_nIndex);
		virtual void UpdateHashId();
		Json::Value & operator[](const std::string & a_Property);
		Json::Value & GetProperties();
		void SetProperties(const Json::Value & a_Properties, bool a_bUpdateHash = true );
		void SetHashId(const std::string & a_HashId);
		void SetWeight(float a_fWeight);
		void SetTime( double a_fTime );

		void SetSource(const IVertex::SP & a_spSource);
		void SetDestination(const IVertex::SP & a_spDest);
		void SetDirty( bool a_bDirty );

	protected:
		//! Data
		VertexIndex			m_nIndex;
		HashId				m_HashId;
		Json::Value			m_Properties;
		float				m_fWeight;
		double				m_fTime;
		IVertex::WP			m_Source;
		IVertex::WP			m_Destination;
		bool				m_bDirty;
	};

	//! This object is created the traverse the graph and search for vertex/edge objects. These objects
	//! are usually chained together to form a more complex query, this is done by linking each ITraverser object
	//! to the next object in the chain to form the query.

	//! This object holds it's results in a vector of vertex indexes into the original graph object.
	class SELF_API ITraverser : public ISerializable, 
		public boost::enable_shared_from_this<ITraverser>
	{
	public:
		RTTI_DECL();

		//! Types
		typedef boost::shared_ptr<ITraverser>		SP;
		typedef boost::weak_ptr<ITraverser>			WP;

		typedef std::vector<VertexIndex>			IndexList;
		typedef Delegate<SP>						TraverseCallback;

		//! Construction
		ITraverser()
		{}
		ITraverser(const Condition & a_Condition, const SP & a_spNext) :
			m_wpGraph(a_spNext ? a_spNext->m_wpGraph : IGraph::WP()),
			m_spNext(a_spNext),
			m_spCondition(a_Condition.Clone())
		{}
		virtual ~ITraverser()
		{}

		//! ISerializable interface
		virtual void	Serialize(Json::Value & json);
		virtual void	Deserialize(const Json::Value & json);

		//! Accessors
		IGraph::SP		GetGraph() const;					// returns the owning graph
		SP				GetNext() const;					// returns the next ITraverser object

		const IndexList & GetVertexList() const;
		VertexIndex		operator[]( size_t a_Vertex ) const;	

		size_t			GetVertexCount() const;
		IVertex::SP		GetVertex( size_t a_Vertex ) const;	// helper to grab the graph object and pull the vertex object from the graph

		//! Mutators
		void			SetGraph(const IGraph::SP & a_spGraph);

		//! Interface
		virtual SP		Filter(const Condition & a_spCond) = 0;						//! Get all verts that meet the provided conditions
		virtual SP		Out(const Condition & a_Condition = NULL_CONDITION) = 0;	//! Get all verts with an out edge that meets the provided conditions
		virtual SP		In(const Condition & a_spCond = NULL_CONDITION) = 0;		//! Get all verts with an in edge that meets the provided conditions

		//! Create a sub-graph from the the provided ITraverser, this is useful for creating a smaller
		//! chunk of a graph that can be serialized over a network. The created graph will only contain
		//! vertex/edge data that the traverser is referencing. The user may then use the MergeGraph()
		//! to merge the created graph into a another graph to combine data.
		virtual IGraph::SP CreateGraph( const std::string & a_GraphId ) = 0;

		//! Traverse the graph and invoke the provided callback once the search is completed. Since traverse
		//! make take some time to complete, this call is asynchronous.
		bool			Start(TraverseCallback a_Callback, bool a_bNotifyObserver = true );
		//! Invoke to continue this traverser without notifying the observer
		void			Continue();

	protected:
		//! Data
		IGraph::WP		m_wpGraph;
		IndexList		m_Verts;

		SP				m_spNext;
		Condition::SP	m_spCondition;
		TraverseCallback m_Callback;

		void			OnNextDone(SP a_spTraverser);

		//! Actually perform the traverse
		virtual void	OnTraverse() = 0;
	};

	class SELF_API IObserver 
	{
	public:
		virtual bool OnStartTraverse( const ITraverser::SP & a_spTraverser ) { return false; }
		virtual void OnCreateVertex( const IVertex::SP & a_spVertex ) {}
		virtual void OnRemoveVertex( const IVertex::SP & a_pVertex ) {}
		virtual void OnCreateEdge( const IEdge::SP & a_spEdge ) {}
		virtual void OnRemoveEdge( const IEdge::SP & a_spEdge ) {}
	};

	//! Construction
	IGraph() : m_pObserver( NULL )
	{}
	IGraph( const std::string & a_GraphId ) : m_GraphId( a_GraphId ), m_pObserver( NULL )
	{}
	virtual ~IGraph()
	{}

	const std::string & GetGraphId() const { return m_GraphId; }
	IObserver *			GetObserver() const { return m_pObserver; }
	void				SetObserver( IObserver * a_pObserver) { m_pObserver = a_pObserver; }

	//! Import a graph from a flat file.
	virtual bool		Import( const std::string & a_File ) = 0;
	//! Export this graph into a flat file.
	virtual bool		Export( const std::string & a_File ) = 0;
	//! Load this graph from storage.
	virtual bool		Load( OnGraphLoaded a_OnGraphLoaded = OnGraphLoaded() ) = 0;
	//! Save any changes to this graph to storage.
	virtual bool		Save() = 0;
	//! Close this graph storage, any unsaved changes will be lost.
	virtual bool		Close() = 0;
	//! Clear this graph of all vertices and edges
	virtual void		Clear() = 0;
	//! Optimize the graph
	virtual void		Optimize() = 0;

	//! Accessors for the contained data - NOTE: Removing a vertex/edges do not reduce the vertex/edge count. 
	//! Those removed vertex/edge's will return a NULL if you call GetVertex/GetEdge.
	//! Calling Optimize() will re-pack the graph and the vertex/edge count will be accurate.
	virtual size_t		GetEdgeCount() const = 0;
	virtual IEdge::SP	GetEdgeByIndex(VertexIndex a_nIndex) const = 0;
	virtual size_t		GetVertexCount() const = 0;
	virtual IVertex::SP GetVertexByIndex(VertexIndex a_nIndex) const = 0;
	
	//! Add the provided graph into this graph. This routine will not add a new vertex if another vertex
	//! already exists with the same hash ID. The same goes for edges connecting vertex objects, new edges
	//! will not be added between vertex objects if another edge already exists with the same hash id.
	virtual void		MergeGraph(const IGraph::SP & a_spGraph ) = 0;
	//! Create a new traverser object for searching this graph.
	virtual ITraverser::SP CreateTraverser(const Condition & a_spCond = NULL_CONDITION) = 0;
	//! Find a edge between the two provided vertex objects and the given hash ID. 
	virtual IEdge::SP	FindEdge( const IVertex::SP & a_Src,
							const IVertex::SP & a_Dest,
							const HashId & a_HashId ) = 0;
	//! Find/Create a new edge for this graph.
	virtual IEdge::SP	GetEdge(const IVertex::SP & a_Src, 
							const IVertex::SP & a_Dest,
							const Json::Value & a_Properties = Json::Value(),
							float a_fWeight = 1.0f ) = 0;
	//! Remove a edge from this graph.
	virtual bool		RemoveEdge(const IEdge::SP & a_spEdge ) = 0;
	//! Find the provided vertex by it's hash ID
	virtual IVertex::SP FindVertex(const std::string & a_HashId) = 0;
	//! Find/Create a new vertex in this graph, returns the IVert object.
	virtual IVertex::SP GetVertex(const Json::Value & a_Properties ) = 0;
	//! Add a existing vertex into this graph.
	virtual bool		AddVertex(const IVertex::SP & a_spVertex ) = 0;
	//! Remove a vertex from this graph.
	virtual bool		RemoveVertex(const IVertex::SP & a_spVertex ) = 0;

	//! Dump graph into string for debugging purposes.
	virtual std::string ToString() = 0;

	//! Helpers
	IVertex::SP operator[](VertexIndex a_nIndex) const
	{
		return GetVertexByIndex(a_nIndex);
	}

	IVertex::SP GetVertexKV(const std::string & a_Key, const Json::Value & a_Value )
	{
		Json::Value props(Json::objectValue);
		props[a_Key] = a_Value;

		return GetVertex(props);
	}

	IEdge::SP GetEdge(const IVertex::SP & a_Src, const IVertex::SP & a_Dst,
		const char * a_pLabel, float a_fWeight = 1.0f)
	{
		Json::Value obj;
		obj["label"] = a_pLabel;

		return GetEdge(a_Src, a_Dst, obj, a_fWeight );
	}

	void Connect(const IVertex::SP & a_Src, const IVertex::SP & a_Dst,
		const Json::Value & a_Properties, float a_fWeight = 1.0f)
	{
		GetEdge(a_Src, a_Dst, a_Properties, a_fWeight);
		GetEdge(a_Dst, a_Src, a_Properties, a_fWeight);
	}

	void Connect(const IVertex::SP & a_Src, const IVertex::SP & a_Dst,
		const char * a_pLabel, float a_fWeight = 1.0f)
	{
		GetEdge(a_Src, a_Dst, a_pLabel, a_fWeight);
		GetEdge(a_Dst, a_Src, a_pLabel, a_fWeight);
	}

protected:
	std::string			m_GraphId;
	IObserver *			m_pObserver;
};

//----------------------------

class SELF_API LabelCondition : public EqualityCondition
{
public:
	RTTI_DECL();

	LabelCondition()
	{}
	LabelCondition(const std::string & a_Label) : EqualityCondition("label", Logic::EQ, a_Label)
	{}
};

#include "IGraphImpl.h"

#endif // SELF_IGRAPH_H
