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

#ifndef SELF_IGRAPH_IMPL_H
#define SELF_IGRAPH_IMPL_H

#include "utils/JsonHelpers.h"
#include "utils/Time.h"

//------------------------------

inline IGraph * IGraph::IVertex::GetGraph() const
{
	return m_pGraph;
}

inline IGraph::VertexIndex IGraph::IVertex::GetIndex() const
{
	return m_nIndex;
}

inline const std::string & IGraph::IVertex::GetHashId() const
{
	return m_HashId;
}

inline const Json::Value & IGraph::IVertex::operator[]( const std::string & a_Property ) const
{
	return m_Properties[ a_Property ];
}

inline const Json::Value & IGraph::IVertex::GetProperties() const
{
	return m_Properties;
}

inline double IGraph::IVertex::GetTime() const
{
	return m_fTime;
}

inline const IGraph::EdgeList & IGraph::IVertex::GetInEdges() const
{
	return m_InEdges;
}

inline const IGraph::EdgeList & IGraph::IVertex::GetOutEdges() const
{
	return m_OutEdges;
}

inline bool IGraph::IVertex::IsDirty() const
{
	return m_bDirty;
}

inline void IGraph::IVertex::SetGraph(IGraph * a_pGraph)
{
	m_pGraph = a_pGraph;
}

inline void IGraph::IVertex::SetIndex( VertexIndex a_nIndex )
{
	if ( m_nIndex != a_nIndex )
	{
		m_nIndex = a_nIndex;
		m_bDirty = true;
	}
}

inline void IGraph::IVertex::UpdateHashId()
{
	SetHashId( JsonHelpers::Hash(m_Properties) );
}

inline Json::Value & IGraph::IVertex::operator[]( const std::string & a_Property )
{
	return m_Properties[ a_Property ];
}

inline Json::Value & IGraph::IVertex::GetProperties()
{
	return m_Properties;
}

inline void IGraph::IVertex::SetProperties( const Json::Value & a_Properties, bool a_bUpdateHash /*= true*/)
{
	m_Properties = a_Properties;
	if ( a_bUpdateHash )
		UpdateHashId();
}

inline void IGraph::IVertex::SetHashId(const std::string & a_HashId)
{
	if ( a_HashId != m_HashId )
	{
		m_HashId = a_HashId;
		m_bDirty = true;
	}
}

inline void IGraph::IVertex::SetTime(double a_fTime)
{
	m_fTime = a_fTime;
	m_bDirty = true;
}

inline void IGraph::IVertex::AddInEdge( const EdgeSP & a_spEdge )
{
	m_InEdges.push_back( a_spEdge );
}

inline bool IGraph::IVertex::RemoveInEdge( const EdgeSP & a_spEdge )
{
	for(size_t i=0;i<m_InEdges.size();++i)
	{
		if ( m_InEdges[i] == a_spEdge )
		{
			m_InEdges.erase( m_InEdges.begin() + i );
			return true;
		}
	}

	return false;
}

inline void IGraph::IVertex::AddOutEdge( const EdgeSP & a_spEdge )
{
	m_OutEdges.push_back( a_spEdge );
}

inline bool IGraph::IVertex::RemoveOutEdge( const EdgeSP & a_spEdge )
{
	for(size_t i=0;i<m_OutEdges.size();++i)
	{
		if ( m_OutEdges[i] == a_spEdge )
		{
			m_OutEdges.erase( m_OutEdges.begin() + i );
			return true;
		}
	}

	return false;
}

inline void IGraph::IVertex::SetDirty( bool a_bDirty )
{
	m_bDirty = a_bDirty;
}

//----------------------------------

inline IGraph::VertexIndex IGraph::IEdge::GetIndex() const
{
	return m_nIndex;
}

inline const std::string & IGraph::IEdge::GetHashId() const
{
	return m_HashId;
}

inline const Json::Value & IGraph::IEdge::operator[]( const std::string & a_Property ) const
{
	return m_Properties[ a_Property ];
}

inline const Json::Value & IGraph::IEdge::GetProperties() const
{
	return m_Properties;
}

inline float IGraph::IEdge::GetWeight() const
{
	return m_fWeight;
}

inline double IGraph::IEdge::GetTime() const
{
	return m_fTime;
}

inline IGraph::IVertex::SP IGraph::IEdge::GetSource() const
{
	return m_Source.lock();
}

inline IGraph::IVertex::SP IGraph::IEdge::GetDestination() const
{
	return m_Destination.lock();
}

inline bool IGraph::IEdge::IsDirty() const
{
	return m_bDirty;
}

inline void IGraph::IEdge::SetIndex(VertexIndex a_nIndex)
{
	if (m_nIndex != a_nIndex )
	{
		m_nIndex = a_nIndex;
		m_bDirty = true;
	}
}

inline void IGraph::IEdge::UpdateHashId()
{
	SetHashId( JsonHelpers::Hash(m_Properties) );
}

inline Json::Value & IGraph::IEdge::operator[]( const std::string & a_Property ) 
{
	return m_Properties[ a_Property ];
}

inline Json::Value & IGraph::IEdge::GetProperties()
{
	return m_Properties;
}

inline void IGraph::IEdge::SetProperties( const Json::Value & a_Properties, bool a_bUpdateHash /*= true*/)
{
	m_Properties = a_Properties;
	if (a_bUpdateHash )
		UpdateHashId();
}

inline void IGraph::IEdge::SetHashId(const std::string & a_HashId)
{
	if ( m_HashId != a_HashId )
	{
		m_HashId = a_HashId;
		m_bDirty = true;
	}
}

inline void IGraph::IEdge::SetWeight(float a_fWeight)
{
	m_fWeight = a_fWeight;
	m_fTime = Time().GetEpochTime();
}

inline void IGraph::IEdge::SetTime( double a_fTime )
{
	m_fTime = a_fTime;
	m_bDirty = true;
}

inline void IGraph::IEdge::SetSource( const IVertex::SP & a_spSource )
{
	m_Source = a_spSource;
}

inline void IGraph::IEdge::SetDestination( const IVertex::SP & a_spDest )
{
	m_Destination = a_spDest;
}

inline void IGraph::IEdge::SetDirty( bool a_bDirty )
{
	m_bDirty = a_bDirty;
}

//----------------------------------

inline IGraph::SP IGraph::ITraverser::GetGraph() const
{
	return m_wpGraph.lock();
}

inline IGraph::ITraverser::SP IGraph::ITraverser::GetNext() const
{
	return m_spNext;
}

inline const IGraph::ITraverser::IndexList & IGraph::ITraverser::GetVertexList() const
{
	return m_Verts;
}

inline IGraph::VertexIndex IGraph::ITraverser::operator[]( size_t a_Vertex ) const
{
	return m_Verts[ a_Vertex ];
}

inline size_t IGraph::ITraverser::GetVertexCount() const
{
	return m_Verts.size();
}

inline IGraph::IVertex::SP IGraph::ITraverser::GetVertex( size_t a_Vertex ) const
{
	IGraph::SP spGraph = m_wpGraph.lock();
	if ( spGraph )
		return spGraph->GetVertexByIndex( m_Verts[ a_Vertex ] );

	return IVertex::SP();
}

inline void IGraph::ITraverser::SetGraph(const IGraph::SP & a_spGraph)
{
	m_wpGraph = a_spGraph;
	if ( m_spNext )
		m_spNext->SetGraph( a_spGraph );
}

#endif // SELF_IGRAPH_H
