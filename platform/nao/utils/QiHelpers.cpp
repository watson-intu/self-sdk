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
#include "QiHelpers.h"

std::string QiHelpers::DumpValue(qi::AnyReference ref)
{
	// start the returned string with a tabs based on the depth into the object..
	std::string rv;
	switch (ref.kind())
	{
		case qi::TypeKind_Int:
			rv += StringUtil::Format("Int=%d", ref.toInt());
			break;
		case qi::TypeKind_Float:
			rv += StringUtil::Format("Float=%f", ref.toFloat());
			break;
		case qi::TypeKind_String:
			rv += StringUtil::Format("String=%s", ref.toString().c_str());
			break;
		case qi::TypeKind_List:
		{
			rv += "{";
			for (size_t i = 0; i < ref.size(); ++i)
			{
				if (i > 0)
					rv += ",";
				rv += StringUtil::Format("List[%u]=", i) + DumpValue(ref[i]);
			}
			rv += "}";
		}
			break;
		case qi::TypeKind_Map:
		{
			std::map<qi::AnyReference, qi::AnyReference> map = ref.asMapValuePtr();

			rv += "{";
			bool bFirst = true;
			for (std::map<qi::AnyReference, qi::AnyReference>::iterator iMap = map.begin(); iMap != map.end(); ++iMap)
			{
				if (!bFirst)
					rv += ",";
				rv += "Map[" + DumpValue(iMap->first) + "]=";
				rv += DumpValue(iMap->second);
				bFirst = false;
			}
			rv += "}";
		}
			break;
		case qi::TypeKind_Object:
			rv += "Object";
			break;
		case qi::TypeKind_Pointer:
			rv += "Pointer";
			break;
		case qi::TypeKind_Dynamic:
			rv += "Dynamic={" + DumpValue(ref.content()) + "}";
			break;
		case qi::TypeKind_Raw:
			rv += "Raw";
			break;
		case qi::TypeKind_Iterator:
			rv += "Iterator";
			break;
		case qi::TypeKind_Function:
			rv += "Function";
			break;
		case qi::TypeKind_Signal:
			rv += "Signal";
			break;
		case qi::TypeKind_Property:
			rv += "Property";
			break;
		case qi::TypeKind_VarArgs:
			rv += "VarArgs";
			break;
		default:
			rv += "?";
			break;
	}

	return rv;
}

std::string QiHelpers::DumpObject(qi::AnyObject obj)
{
	std::string dump = "Dumping Object: " + obj.metaObject().description() + "\n";
	qi::MetaObject::MethodMap methods = obj.metaObject().methodMap();
	for (qi::MetaObject::MethodMap::iterator iMethod = methods.begin(); iMethod != methods.end(); ++iMethod)
		dump += "Method: " + iMethod->second.name() + "\n";
	qi::MetaObject::SignalMap signals = obj.metaObject().signalMap();
	for (qi::MetaObject::SignalMap::iterator iSignal = signals.begin(); iSignal != signals.end(); ++iSignal)
		dump += "Signal: " + iSignal->second.name() + "\n";
	qi::MetaObject::PropertyMap props = obj.metaObject().propertyMap();
	for (qi::MetaObject::PropertyMap::iterator iProp = props.begin(); iProp != props.end(); ++iProp)
		dump += "Property: " + iProp->second.name() + "\n";

	return dump;
}

