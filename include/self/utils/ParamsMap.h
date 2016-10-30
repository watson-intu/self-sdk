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

#ifndef SKILLPARAMS_H
#define SKILLPARAMS_H

#include <map>
#include <string>

#include "jsoncpp/json/json.h"
#include "utils/ISerializable.h"
#include "utils/JsonHelpers.h"
#include "SelfLib.h"		// include last always

//! What character is used a seperator for paths
#define PARAMS_PATH_SEPERATOR		'/'

//! This is a container for skill/gesture arguments.
class SELF_API ParamsMap : public ISerializable
{
public:
	RTTI_DECL();

	//! Construction
	ParamsMap()
	{
		m_Data["Type_"] = "ParamsMap";
	}
	ParamsMap( const std::string & key, const std::string & value )
	{
		m_Data[key] = value;
	}
	ParamsMap( const std::string & key, float value )
	{
		m_Data[key] = value;
	}
	ParamsMap( const ParamsMap & a_Copy ) : m_Data( a_Copy.m_Data )
	{}
	ParamsMap( const Json::Value & a_json ) : m_Data( a_json )
	{}

	//! Accessors
	const Json::Value & GetData() const
	{
		return m_Data;
	}

	//! Validate a given path, returns false if any member of the path doesn't exist.
	bool ValidPath(const std::string & a_Path) const
	{
		return JsonHelpers::ValidPath(m_Data, a_Path);
	}

	const Json::Value & operator[](const std::string & a_Path) const
	{
		return JsonHelpers::Resolve(m_Data, a_Path);
	}

	//! Resolve a full path to a value within this ParamsMap. If the value
	//! doesn't exist then it a null value will be created and returned. 
	Json::Value & operator[](const std::string & a_Path) 
	{
		return JsonHelpers::Resolve(m_Data, a_Path);
	}

	//! Merge the provided ParamsMap into this map, if a_bReplace is false, then no matching keys will be replaced.
	void Merge( const ParamsMap & a_Merge, bool a_bReplace = true )
	{
		JsonHelpers::Merge(m_Data, a_Merge.m_Data, a_bReplace);
	}

	//! Resolve any variables in the provides string from this params map. 
	//! Variable names start with { and end with }. 
	std::string ResolveVariables(const std::string & a_Input);
	//! Resolve variables found in the provided Json, return the translated json.
	Json::Value ResolveVariables(const Json::Value & a_Json);

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

private:
	//! Data
	Json::Value		m_Data;
};

#endif

