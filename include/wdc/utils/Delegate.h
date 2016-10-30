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

#ifndef WDC_DELEGATE_H
#define WDC_DELEGATE_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#include "WDCLib.h"

#ifndef ENABLE_DELEGATE_DEBUG
#if defined(_DEBUG) || !defined(__OPTIMIZE__)
#define ENABLE_DELEGATE_DEBUG 1
#else
#define ENABLE_DELEGATE_DEBUG 0
#endif
#endif

//! This delegate class allows you to store a function call to object. This is useful for passing a callback
//! around through functions or storing that callback as a data member in a class.
//!
//! Delegate<int> d = Delegate<int>::Create<MyObject,&MyObject::Func>( pObject );
//! 
//! d( 42 );		// invoke the delegate function
//!

template<typename ARG>
class Delegate
{
public:
	Delegate() : m_pObject(0), m_pStub(0)
	{}

	template <class T, void (T::*TMethod)(ARG)>
#if !ENABLE_DELEGATE_DEBUG
	static Delegate Create(T* object_ptr)
#else
	static Delegate Create(T* object_ptr, const char * a_pFile, int a_nLine )
#endif
	{
		Delegate d;
		if ( object_ptr != 0 )
		{
			d.m_pObject = object_ptr;
			d.m_pStub = &method_stub<T, TMethod>; // #1
#if ENABLE_DELEGATE_DEBUG
			d.m_pFile = a_pFile;
			d.m_nLine = a_nLine;
#endif
		}
		return d;
	}

	template <class T, void (T::*TMethod)(ARG)>
#if !ENABLE_DELEGATE_DEBUG
	static Delegate Create( boost::shared_ptr<T> object_ptr )
#else
	static Delegate Create( boost::shared_ptr<T> object_ptr, const char * a_pFile, int a_nLine )
#endif
	{
		Delegate d;
		if ( object_ptr != 0 )
		{
			d.m_pObject = object_ptr.get();
			d.m_WeakPtr = boost::weak_ptr<T>( object_ptr );
			d.m_pStub = &method_stub<T, TMethod>; // #1
#if ENABLE_DELEGATE_DEBUG
			d.m_pFile = a_pFile;
			d.m_nLine = a_nLine;
#endif
		}
		return d;
	}

	bool operator()(ARG a1) const
	{
		if (m_pStub != 0) 
			return (*m_pStub)(m_pObject, m_WeakPtr, a1);
		return false;
	}

	void Reset()
	{
		m_pObject = 0;
		m_WeakPtr.clear();
		m_pStub = 0;
	}

	bool IsValid() const
	{
		return m_pObject != 0 && m_pStub != 0;
	}

	bool IsObject(void * obj) const
	{
		return obj == m_pObject;
	}

#if ENABLE_DELEGATE_DEBUG
	const char * GetFile() const
	{
		return m_pFile;
	}

	int GetLine() const
	{
		return m_nLine;
	}
#endif

private:
	typedef bool (*stub_type)(void* object_ptr, const boost::any & weak_ptr, ARG);

	void *			m_pObject;
	boost::any		m_WeakPtr;
	stub_type		m_pStub;
#if ENABLE_DELEGATE_DEBUG
	const char *	m_pFile;
	int				m_nLine;
#endif

	template <class T, void (T::*TMethod)(ARG)>
	static bool method_stub(void* object_ptr, const boost::any & weak_ptr, ARG a1)
	{
		if (! weak_ptr.empty() )
			object_ptr = boost::any_cast< boost::weak_ptr<T> >( weak_ptr ).lock().get();
		if ( object_ptr == 0 )
			return false;

		T* p = reinterpret_cast<T*>(object_ptr);
		(p->*TMethod)(a1); // #2
		return true;
	}
};

//! Helper macro for making a delegate a little bit less wordy, e.g.
//! Delegate<int> d = DELEGATE( int, MyObject, Func, pObject );
#if !ENABLE_DELEGATE_DEBUG
#define DELEGATE( CLASS, FUNC, ARG, OBJ )		Delegate<ARG>::Create<CLASS,&CLASS::FUNC>( OBJ )
#else
#define DELEGATE( CLASS, FUNC, ARG, OBJ )		Delegate<ARG>::Create<CLASS,&CLASS::FUNC>( OBJ, __FILE__, __LINE__ )
#endif

//! Delegate for a function that doesn't take any arguments.
class VoidDelegate
{
public:
	VoidDelegate() : m_pObject(0),
		m_pStub(0)
	{}

	template <class T, void (T::*TMethod)()>
#if !ENABLE_DELEGATE_DEBUG
	static VoidDelegate Create(T* object_ptr)
#else
	static VoidDelegate Create(T* object_ptr, const char * a_pFile, int a_nLine)
#endif
	{
		VoidDelegate d;
		if ( object_ptr != 0 )
		{
			d.m_pObject = object_ptr;
			d.m_pStub = &method_stub<T, TMethod>; // #1
#if ENABLE_DELEGATE_DEBUG
			d.m_pFile = a_pFile;
			d.m_nLine = a_nLine;
#endif
		}
		return d;
	}

	template <class T, void (T::*TMethod)()>
#if !ENABLE_DELEGATE_DEBUG
	static VoidDelegate Create(boost::shared_ptr<T> object_ptr)
#else
	static VoidDelegate Create(boost::shared_ptr<T> object_ptr, const char * a_pFile, int a_nLine)
#endif
	{
		VoidDelegate d;
		if ( object_ptr != 0 )
		{
			d.m_pObject = object_ptr.get();
			d.m_WeakPtr = boost::weak_ptr<T>( object_ptr );
			d.m_pStub = &method_stub<T, TMethod>; // #1
#if ENABLE_DELEGATE_DEBUG
			d.m_pFile = a_pFile;
			d.m_nLine = a_nLine;
#endif
		}
		return d;
	}

	bool operator()() const
	{
		if ( m_pStub != 0 )
			return (*m_pStub)(m_pObject, m_WeakPtr);
		return false;
	}

	void Reset()
	{
		m_pObject = 0;
		m_WeakPtr.clear();
		m_pStub = 0;
	}

	bool IsValid() const
	{
		return m_pObject != 0 && m_pStub != 0;
	}

	bool IsObject(void * obj) const
	{
		return obj == m_pObject;
	}

#if ENABLE_DELEGATE_DEBUG
	const char * GetFile() const
	{
		return m_pFile;
	}

	int GetLine() const
	{
		return m_nLine;
	}
#endif
private:
	typedef bool (*stub_type)(void* object_ptr, const boost::any & );

	void*		m_pObject;
	boost::any	m_WeakPtr;
	stub_type	m_pStub;
#if ENABLE_DELEGATE_DEBUG
	const char * m_pFile;
	int			m_nLine;
#endif

	template <class T, void (T::*TMethod)()>
	static bool method_stub(void* object_ptr, const boost::any & weak_ptr )
	{
		if (! weak_ptr.empty() )
			object_ptr = boost::any_cast< boost::weak_ptr<T> >( weak_ptr ).lock().get();
		if ( object_ptr == 0 )
			return false;

		T * p = reinterpret_cast<T*>(object_ptr);
		(p->*TMethod)(); // #2
		return true;
	}
};

//! Helper macro for making a delegate a little bit less wordy, e.g.
//! Delegate<int> d = DELEGATE( int, MyObject, Func, pObject );
#if !ENABLE_DELEGATE_DEBUG
#define VOID_DELEGATE( CLASS, FUNC, OBJ )		VoidDelegate::Create<CLASS,&CLASS::FUNC>( OBJ )
#else
#define VOID_DELEGATE( CLASS, FUNC, OBJ )		VoidDelegate::Create<CLASS,&CLASS::FUNC>( OBJ, __FILE__, __LINE__ )
#endif

//------------------------------------------------------------

template<typename ARG>
class DelegateList
{
public:
	//! Types
	typedef std::list< Delegate<ARG> >		List;

	//! Accessors
	const List & GetList() const 
	{
		return m_Delegates;
	}

	//! Mutators
	void Add( Delegate<ARG> a_Delegate )
	{
		m_Delegates.push_back( a_Delegate );
	}

	bool Remove( void * a_pObject )
	{
		for( typename List::iterator iDelegate = m_Delegates.begin(); iDelegate != m_Delegates.end(); ++iDelegate )
		{
			if ( (*iDelegate).IsObject( a_pObject ) )
			{
				m_Delegates.erase( iDelegate );
				return true;
			}
		}
		return false;
	}

	void Invoke( ARG a_Arg )
	{
		for( typename List::iterator iDelegate = m_Delegates.begin(); iDelegate != m_Delegates.end(); ++iDelegate )
			(*iDelegate)( a_Arg );
	}

	void Clear()
	{
		m_Delegates.clear();
	}

private:
	//! Data
	List	m_Delegates;
};

class VoidDelegateList
{
public:
	//! Types
	typedef std::list<VoidDelegate>		List;

	//! Accessors
	const List & GetList() const 
	{
		return m_Delegates;
	}

	//! Mutators
	void Add( VoidDelegate a_Delegate )
	{
		m_Delegates.push_back( a_Delegate );
	}

	bool Remove( void * a_pObject )
	{
		for( List::iterator iDelegate = m_Delegates.begin(); iDelegate != m_Delegates.end(); ++iDelegate )
		{
			if ( (*iDelegate).IsObject( a_pObject ) )
			{
				m_Delegates.erase( iDelegate );
				return true;
			}
		}
		return false;
	}

	void Invoke()
	{
		for( List::iterator iDelegate = m_Delegates.begin(); iDelegate != m_Delegates.end(); ++iDelegate )
			(*iDelegate)();
	}

	void Clear()
	{
		m_Delegates.clear();
	}

private:
	//! Data
	List	m_Delegates;
};

#endif


