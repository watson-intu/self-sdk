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

#ifndef FEATURE_MANAGER_H
#define FEATURE_MANAGER_H

#include <list>

#include "utils/Factory.h"
#include "IFeatureExtractor.h"
#include "SelfLib.h"			// include last

//! This feature manager manages all Feature extractors, those feature extractors push
//! data into the Blackboard object.
class SELF_API FeatureManager
{
public:
	//! Types
	typedef std::list< IFeatureExtractor::SP >	FeatureExtractorList;
	typedef Factory< IFeatureExtractor >		FeatureExtractorFactory;

	//! Construction
	FeatureManager();
	~FeatureManager();

	//! Accessors
	const FeatureExtractorList &	
								GetFeatureExtractorList() const;

	//! Start this manager, initializes all available Classifer objects.
	bool						Start();
	//! Stop this manager.
	bool						Stop();

	template<typename T>
	T * FindExtractor() const
	{
		for( FeatureExtractorList::const_iterator iExtractor = m_FeatureExtractors.begin(); 
		iExtractor != m_FeatureExtractors.end(); ++iExtractor )
		{
			T * pExtractor = DynamicCast<T>( (*iExtractor).get() );
			if ( pExtractor != NULL )
				return pExtractor;
		}
		return NULL;
	}

	template<typename T>
	T * GetExtractor() 
	{
		T * pExtractor = FindExtractor<T>();
		if ( pExtractor == NULL )
		{
			pExtractor = new T();
			if ( pExtractor->OnStart() )
			{
				m_FeatureExtractors.push_back( IFeatureExtractor::SP( pExtractor ) );
				return pExtractor;
			}

			delete pExtractor;
			pExtractor = NULL;
		}

		return pExtractor;
	}

private:
	//!Data
	bool						m_bActive;
	FeatureExtractorList		m_FeatureExtractors;
};

inline const FeatureManager::FeatureExtractorList & FeatureManager::GetFeatureExtractorList() const
{
	return m_FeatureExtractors;
}

#endif