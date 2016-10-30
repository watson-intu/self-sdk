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

#ifndef WDC_LIBRARY_H
#define WDC_LIBRARY_H

#include <string>
#include "WDCLib.h"

class WDC_API Library
{
public:
	Library();
	Library( const Library & a_Copy );
	Library( const std::string & a_Lib );
	~Library();

	bool IsLoaded() const
	{
		return m_pLibrary != NULL;
	}

	void Load( const std::string & a_Lib );
	void Unload();

private:
	std::string m_Lib;
	void * m_pLibrary;

};

#endif

