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

#include "utils/StringUtil.h"
#include "AlHelpers.h"

#ifndef _WIN32
std::string AlHelpers::DumpValue( const AL::ALValue & a_Value )
{
    std::string r;
    if ( a_Value.isArray() )
    {
        r += "array: [ ";
        for(int i=0;i<a_Value.getSize();++i)
            r += DumpValue( a_Value[i] );
        r += "] ";
    }
    else if ( a_Value.isBool() )
        r += "bool: " + ((bool)a_Value) ? "true" : "false";
    else if ( a_Value.isInt() )
        r += "int:" + StringUtil::Format( "%d", ((int)a_Value) );
    else if ( a_Value.isFloat() )
        r += "float:" + StringUtil::Format( "%g", ((float)a_Value) );
    else if ( a_Value.isString() )
        r += "string: " + ((std::string)a_Value);
    else if ( a_Value.isBinary() )
        r += StringUtil::Format( "binary: %u bytes", a_Value.getSize() );
    else if ( a_Value.isObject() )
        r += "object";

    return r;
}

std::string AlHelpers::ToString(const AL::ALValue & a_Value)
{
	if (a_Value.isBool())
		return ((bool)a_Value) ? "true" : "false";
	else if (a_Value.isInt())
		return StringUtil::Format("%d", ((int)a_Value));
	else if (a_Value.isFloat())
		return StringUtil::Format("%g", ((float)a_Value));
	else if (a_Value.isString())
		return ((std::string)a_Value);

	return "?";
}

#endif