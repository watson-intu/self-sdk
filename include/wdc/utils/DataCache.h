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

#ifndef WDC_DATA_CACHE_H
#define WDC_DATA_CACHE_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

#include "Log.h"
#include "StringUtil.h"

#include "WDCLib.h"

//! Simply binary cache for use by the services. The caches data into the local file system
class WDC_API DataCache : public boost::enable_shared_from_this<DataCache>
{
public:
	//! Types
	struct CacheItem
	{
		CacheItem() : m_Time(0.0), m_Size(0), m_bLoaded(false)
		{}

		std::string		m_Path;			// fully path to item
		std::string		m_Id;			// id of item
		double			m_Time;			// epoch time of cache item
		unsigned int	m_Size;			// size of item in bytes
		bool			m_bLoaded;		// true if loaded
		std::string		m_Data;			// data of item
	};
	typedef boost::shared_ptr<DataCache>			SP;

	//! Construction
	DataCache();

	//! Accessors
	bool IsInitialized() const
	{
		return m_bInitialized;
	}

	//! Initialize this cache
	bool Initialize( const std::string & a_CachePath, 
		unsigned int a_MaxCacheSize = 1024 * 1024 * 50,
		double a_MaxCacheAge = 24 * 7 );
	void Uninitialize();

	//! Find data in this cache by ID, returns a NULL if object is not found in this cache.
	CacheItem * Find( const std::string & a_ID );
	//! Save data into this cache.
	bool Save( const std::string & a_ID, const std::string & a_Data );
	//! Flush an item from this cache.
	bool Flush( const std::string & a_ID );
	//! Flush out aged data from this cache.
	bool FlushAged();
	//! flush the oldest item from the cache.
	bool FlushOldest();
	//! Flush all data from this cache.
	bool FlushAll();

	//! Helpers
	CacheItem * Find(unsigned int a_ID)
	{
		return Find(StringUtil::Format("%8.8x", a_ID));
	}
	bool Save(unsigned int a_ID, const std::string & a_Data)
	{
		return Save(StringUtil::Format("%8.8x", a_ID), a_Data);
	}

private:
	//! Types
	typedef std::map< std::string, CacheItem >		CacheItemMap;

	//! Data
	bool				m_bInitialized;
	std::string			m_CachePath;
	unsigned int		m_MaxCacheSize;
	double				m_MaxCacheAge;
	unsigned int		m_CurrentCacheSize;
	CacheItemMap		m_Cache;
};

#endif

