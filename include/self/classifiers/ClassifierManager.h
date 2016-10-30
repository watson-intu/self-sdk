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

#ifndef CLASSIFIER_MANAGER_H
#define CLASSIFIER_MANAGER_H

#include <list>

#include "utils/Factory.h"
#include "IClassifier.h"
#include "SelfLib.h"		// include last

//! This class manages all active classifier instances. This classifiers subscribe to sensors 
//! and add concepts to the BlackBoard object contained by the SelfInstance.
class SELF_API ClassifierManager
{
public:
	//! Types
	typedef std::list< IClassifier::SP >	ClassifierList;

	//! Construction
	ClassifierManager();
	~ClassifierManager();

	//! Accessors
	const ClassifierList &		GetClassifierList() const;

	//! Start this manager, initializes all available Classifier objects.
	bool						Start();
	//! Stop this manager.
	bool						Stop();

	template<typename T>
	T * FindClassifier() const
	{
		for( ClassifierList::const_iterator iClass = m_Classifiers.begin(); iClass != m_Classifiers.end(); ++iClass )
		{
			T * pClassifier = DynamicCast<T>( (*iClass).get() );
			if ( pClassifier != NULL )
				return pClassifier;
		}
		return NULL;
	}

	template<typename T>
	T * GetClassifier() 
	{
		T * pClassifier = FindClassifier<T>();
		if ( pClassifier == NULL )
		{
			pClassifier = new T();
			if ( pClassifier->OnStart() )
			{
				m_Classifiers.push_back( IClassifier::SP( pClassifier ) );
				return pClassifier;
			}

			delete pClassifier;
			pClassifier = NULL;
		}

		return pClassifier;
	}

private:
	//! Data
	bool						m_bActive;
	ClassifierList				m_Classifiers;
};

inline const ClassifierManager::ClassifierList & ClassifierManager::GetClassifierList() const
{
	return m_Classifiers;
}


#endif

