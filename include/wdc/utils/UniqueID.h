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

#ifndef WDC_GUID_H
#define WDC_GUID_H

#include <string>
#include "WDCLib.h"

class WDC_API UniqueID 
{
public:
	//! Construction
	UniqueID()
	{
		Generate();
	}
	UniqueID( const std::string & a_ID ) : m_GUID( a_ID	 )
	{}

	const std::string & Get() const
	{
		return m_GUID;
	}

	//! Generate a new ID, returns a const reference to the ID contained by this object.
	const std::string & Generate();

private:
	//! Data
	std::string		m_GUID;
};

#endif


