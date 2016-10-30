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

#ifndef WDC_FACTORY_H
#define WDC_FACTORY_H

#include <map>
#include <string>
#include "Log.h"
#include "WDCLib.h"

class ICreator
{
public:
	ICreator( bool a_bOverride ) : m_bOverride( a_bOverride )
	{}
	virtual void * Create() = 0;
	virtual ~ICreator() {}

	bool IsOverride() const
	{
		return m_bOverride;
	}
private:
	bool m_bOverride;
};
typedef std::map<std::string, ICreator *>		CreatorMap;

class WDC_API IFactory
{
public:
	//! Register a specific class type with this factory, it should inherit from the BASE class.
	//! If a_bOverride is true, then we will replace any previously registered class by the same
	//! name. If false, then we will return false if any class is already registered with the same ID.
	bool Register(const std::string & a_ID, ICreator * a_pCreator)
	{
		if (m_CreatorMapping.find(a_ID) != m_CreatorMapping.end())
		{
			if (! a_pCreator->IsOverride() )
				return false;
		}

		// This will generate a compile error if your class doesn't inherit from BASE
		Log::Debug("Factory", "Registered %s in factory.", a_ID.c_str());
		m_CreatorMapping[a_ID] = a_pCreator;
		return true;
	}

	//! Unregister a factory by ID
	virtual bool Unregister(const std::string & a_ID, ICreator * a_pCreator)
	{
		if (a_pCreator == NULL)
			return false;

		CreatorMap::iterator iMapping = m_CreatorMapping.find(a_ID);
		if (iMapping != m_CreatorMapping.end())
		{
			if (a_pCreator == iMapping->second)
			{
				m_CreatorMapping.erase(iMapping);
				return true;
			}
		}

		return false;
	}

	ICreator * FindCreator( const std::string & a_ID )
	{
		CreatorMap::iterator iMapping = m_CreatorMapping.find(a_ID);
		if (iMapping != m_CreatorMapping.end())
			return iMapping->second;

		return NULL;
	}

	bool IsOverride( const std::string & a_ID )
	{
		ICreator * pCreator = FindCreator( a_ID );
		if ( pCreator != NULL )
			return pCreator->IsOverride();
		return false;
	}

protected:
	//! Data
	CreatorMap		m_CreatorMapping;
};

//! Factory pattern for making object by ID.
template<typename BASE>
class Factory : public IFactory
{
public:
	BASE * CreateObject(const std::string & a_ID)
	{
		CreatorMap::iterator iMapping = m_CreatorMapping.find(a_ID);
		if (iMapping != m_CreatorMapping.end())
			return reinterpret_cast<BASE *>( iMapping->second->Create() );

		return NULL;
	}

	void CreateAllObjects( std::list<BASE *> & a_Objects )
	{
		int i = 0;
		for (CreatorMap::iterator iObject = m_CreatorMapping.begin(); iObject != m_CreatorMapping.end(); ++iObject)
			a_Objects.push_back( reinterpret_cast<BASE *>( iObject->second->Create() ) );
	}
};

/*
   The RegisterWithFactory class is a helper class for registering a class with a factory
   on construction, and unregistering with the factory on destruction. Typically, you'd declare
   this object as a global or static variable. e.g.

   static Factory<Widget> & GetWidgetFactory();


   RegisterWithFactory<MyWidget> register( "MyWidget", GetWidgetFactory() );
*/

template< typename T >
class RegisterWithFactory
{
public:
	//! Types
	template<typename BASE, typename K>
	class Creator : public ICreator
	{
	public:
		Creator( bool a_bOverride ) : ICreator( a_bOverride )
		{}

		virtual void * Create()
		{
			// must static case to BASE * before returning void *, so pointer math works correctly, do not change this..
			return static_cast<BASE *>( new K() );
		}
	};

	template< typename BASE >
	RegisterWithFactory( const std::string & a_ID, Factory< BASE > & a_Factory, bool a_bOverride = false ) 
		: m_ID( a_ID ), m_pFactory( &a_Factory ), m_pCreator( new Creator<BASE, T>( a_bOverride ) )
	{
		a_Factory.Register(m_ID, m_pCreator );
	}

	~RegisterWithFactory()
	{
		m_pFactory->Unregister( m_ID, m_pCreator );
		delete m_pCreator;
	}

private:
	//! Data
	std::string		m_ID;
	IFactory *		m_pFactory;
	ICreator *		m_pCreator;
};

//! Helper macro to register with a factory, should be placed in global space unless you want it to unregister
//! once it goes out of scope.
#define REG_FACTORY( CLASS, FACTORY )				RegisterWithFactory<CLASS> register_##CLASS( #CLASS, FACTORY)

//! Override a previously registered factory with a new class by using the same ID.
#define REG_OVERRIDE( OVERRIDE, CLASS, FACTORY )	RegisterWithFactory<CLASS> register_##CLASS( #OVERRIDE, FACTORY, true )

#endif

