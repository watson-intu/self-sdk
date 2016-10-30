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

#ifndef WDC_PATH_H
#define WDC_PATH_H

#include <string>		// std::string

#include "StringUtil.h"
#include "WDCLib.h"

//! Helper class for parsing a file path and extracting the different parts.
class WDC_API Path
{
public:
	//! Construction
	Path()
	{}
	Path( const char * a_pPath ) 
	{
		Parse( a_pPath );
	}
	Path( const std::string & a_Path ) 
	{
		Parse( a_Path );
	}
	Path( const Path & a_Copy ) : 
		m_Directory( a_Copy.m_Directory ),
		m_File( a_Copy.m_File ),
		m_Extension( a_Copy.m_Extension )
	{}

	//! Returns the directory part of this path (.e.g C:/work/file.txt -> C:/work/)
	const std::string & GetDirectory() const
	{
		return m_Directory;
	}
	//! Returns the filename of this path (.e.g C:/work/file.txt -> file)
	const std::string & GetFile() const
	{
		return m_File;
	}
	//! Returns the extension of the file (e.g. C:/work/file.txt -> .txt)
	const std::string & GetExtension() const
	{
		return m_Extension;
	}

	//! get the filename including extension
	std::string GetFileName() const
	{
		return m_File + m_Extension;
	}

	//! Get the full path
	std::string GetFullPath() const
	{
		return m_Directory + m_File + m_Extension;
	}

	void Parse( const std::string & a_Path )
	{
		Reset();

		m_File = a_Path;
		StringUtil::Replace( m_File, "\\", "/" );			// normalize the slash direction to unix 

		size_t nLastSlash = m_File.find_last_of( '/' );
		if (nLastSlash != std::string::npos )
		{
			m_Directory = m_File.substr( 0, nLastSlash + 1 );
			m_File = m_File.substr( nLastSlash + 1 );
		}

		size_t nExt = m_File.find_last_of( '.' );
		if ( nExt != std::string::npos )
		{
			m_Extension = m_File.substr( nExt );
			m_File = m_File.substr( 0, nExt );
		}
	}

	void Reset()
	{
		m_Directory.clear();
		m_File.clear();
		m_Extension.clear();
	}

private:
	//! Data
	std::string			m_Directory;
	std::string			m_File;
	std::string			m_Extension;
};

#endif

