/**
* Copyright 2016 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef WDC_RTTI_H
#define WDC_RTTI_H

#include <string>
#include <list>

#include "boost/shared_ptr.hpp"

#include "StringHash.h"
#include "WDCLib.h"

/*
	RTTI (Run Time Time information) class allows us to determine the type
	of an object at run-time. The user of this system must use the RTTI_DECL() macro
	in the class definition.
*/

class WDC_API RTTI
{
public:
	//! Types
	typedef std::list< RTTI * >			ClassList;

	//! Construction
	RTTI(const std::string & a_ClassName) :
		m_ClassName(a_ClassName),
		m_ClassID(StringHash::DJB(a_ClassName.c_str())),
		m_pBaseClass(NULL)
	{}

	RTTI(const std::string & a_ClassName, RTTI & a_BaseClass) :
		m_ClassName(a_ClassName),
		m_ClassID(StringHash::DJB(a_ClassName.c_str())),
		m_pBaseClass(&a_BaseClass)
	{
		a_BaseClass.m_ChildClasses.push_back(this);
	}

	~RTTI()
	{
		if (m_pBaseClass != NULL)
			m_pBaseClass->m_ChildClasses.remove(this);

		// remove our pointer from any child classes, so they don't get a exception..
		for (ClassList::iterator iChild = m_ChildClasses.begin();
		iChild != m_ChildClasses.end(); ++iChild)
		{
			(*iChild)->m_pBaseClass = NULL;
		}
	}

	const std::string &	GetName() const
	{
		return m_ClassName;
	}
	unsigned int GetClassID() const
	{
		return m_ClassID;
	}
	RTTI * GetBaseClass() const
	{
		return m_pBaseClass;
	}
	const ClassList & GetChildClasses() const
	{
		return m_ChildClasses;
	}

	bool operator==(const RTTI & cmp) const
	{
		return this == &cmp;
	}

	//! This returns true if the given type is derived from this class type.
	bool IsType(const RTTI * a_pType) const
	{
		const RTTI * pType = this;
		while (pType != NULL)
		{
			if (pType == a_pType)
				return true;
			pType = pType->m_pBaseClass;
		}

		return false;
	}

private:
	//! Data
	std::string			m_ClassName;		// the name of this class
	unsigned int		m_ClassID;			// hash of our class name
	RTTI *				m_pBaseClass;		// our base class
	ClassList			m_ChildClasses;		// classes derived from this class
};

//! Use this macro for a class that derives from anther class.
#define RTTI_DECL()										\
	static RTTI & GetStaticRTTI();									\
	virtual RTTI & GetRTTI() const { return GetStaticRTTI(); }

#define RTTI_IMPL_BASE(CLASS) RTTI & CLASS::GetStaticRTTI() { static RTTI rtti(#CLASS); return rtti; }	
#define RTTI_IMPL(CLASS,BASE) RTTI & CLASS::GetStaticRTTI() { static RTTI rtti(#CLASS,BASE::GetStaticRTTI()); return rtti; }	

//! This function can be used to cast one pointer type to another, it will return NULL if the type is not correct.
template<typename T, typename K>
inline T * DynamicCast(K * pObject)
{
	if ( pObject != NULL && pObject->GetRTTI().IsType(&T::GetStaticRTTI()) )
		return static_cast<T *>(pObject);
	return NULL;
}

template<typename T,typename K>
inline boost::shared_ptr<T> DynamicCast( const boost::shared_ptr<K> & a_spObject )
{
	if ( a_spObject && a_spObject->GetRTTI().IsType(&T::GetStaticRTTI()) )
		return boost::static_pointer_cast<T>( a_spObject );
	return boost::shared_ptr<T>();
}

#endif
