/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#ifndef WDC_JSON_HELPERS_H
#define WDC_JSON_HELPERS_H

#include <string>
#include "jsoncpp/json/json.h"
#include "WDCLib.h"

class TiXmlElement;

//! This is a container for skill/gesture arguments.
class WDC_API JsonHelpers
{
public:
	//! Validate a given path, returns false if any member of the path doesn't exist.
	static bool ValidPath(const Json::Value & a_Json, const std::string & a_Path);

	//! Convert XML to JSON
	static void MakeJSON( const TiXmlElement * a_pElement, Json::Value & a_Value );

	//! Get a const reference to a Json::Value following the provided path. Returns a NULL json
	//! if the path isn't valid.
	static const Json::Value & Resolve(const Json::Value & a_Json, const std::string & a_Path);

	//! Resolve a full path to a value within this ParamsMap. If the value
	//! doesn't exist then it a null value will be created and returned. 
	static Json::Value & Resolve(Json::Value & a_Json, const std::string & a_Path);

	//! Merge one JSON into the other, if a_bReplace is true, then any duplicates are replaced, otherwise
	//! we will skip elements if they already exist
	static void Merge(Json::Value & a_MergeInto, const Json::Value & a_Merge, bool a_bReplace = true);

	//! Generate a hash of the provided Json.
	static std::string Hash(const Json::Value & a_Json);
};

#endif

