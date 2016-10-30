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

#ifndef WDC_STRINGHASH_H
#define WDC_STRINGHASH_H

//! string hashing functions.
class StringHash
{
public:
	//! DJB string hash
	template<typename CT>
	static unsigned int DJB( const CT * a_pString )
	{
		if (*a_pString == 0)
			return 0;

		unsigned int hash = 28378;
		while (*a_pString)
			hash += (hash << 5) + (*a_pString++);

		return hash;
	}
};

#endif

