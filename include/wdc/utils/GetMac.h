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

#ifndef WDC_GET_MAC_H
#define WDC_GET_MAC_H

#include <string>
#include <list>
#include "WDCLib.h"

class WDC_API GetMac
{
public:
	static std::string GetMyAddress();
	static std::string GetMyAddress(const std::list<std::string> & a_Patterns);
	static std::string GetIpAddress();
};

#endif

