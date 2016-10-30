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

#ifndef QI_HELPERS_H
#define QI_HELPERS_H

#include <string>
#include "qi/anyvalue.hpp"
#include "qi/anyobject.hpp"

//! Helper functions for working with LIBQI
class QiHelpers
{
public:
	//! This function dumps the contents of the given qi::AnyReference object, this is useful for debugging values
	//! sent back by the LIBQI system using TypeErasure.
	static std::string DumpValue(qi::AnyReference ref );
	//! Dumps the methods, properties, and signals of the given object.
	static std::string DumpObject(qi::AnyObject obj );
};

#endif
