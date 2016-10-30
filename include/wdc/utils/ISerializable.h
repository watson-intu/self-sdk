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

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include "Factory.h"
#include "RTTI.h"
#include "Log.h"

#include "jsoncpp/json/json.h"
#include "WDCLib.h"		// include last

//! Base abstract interface for any class that can be serialized from and into json data.
//!
//! In order to use the serialization system fully you must include the RTTI_DECL() in your class
//! definition and use the REG_SERIALIZABLE() macro in the cpp file for your class, for example:
//!
//! ============ SerializedObject.h ===============
//!
//! class SerializedObject : public ISerializable
//! {
//! public:
//!		RTTI_DECL( SerializedObject, ISerializable );
//! 
//!		virtual void Serialize(Json::Value & json) { ... }
//!		virtual void Deserialize(const Json::Value & json) { ... }
//! };
//!
//! ============= SerializedObject.cpp ============
//!
//! REG_SERIALIZABLE( SerializedObject );

class WDC_API ISerializable
{
public:
	RTTI_DECL();

	//! Construction
	virtual ~ISerializable()
	{}

	//! Save this object into the given JSON
	virtual void Serialize(Json::Value & json) = 0;
	//! Load this object from the given JSON
	virtual void Deserialize(const Json::Value & json) = 0;

	//! Helper function to convert this object into JSON
	Json::Value ToJson()
	{
		Json::Value json;
		Serialize( json );

		return json;
	}

	//! Static factory for all classes that can be serialized/deserialized
	static Factory<ISerializable> & GetSerializableFactory();
	//! deserialize a object from a string, if a_pObject is provided then we try to deserialize into the provided object.
	static ISerializable * DeserializeObject( const std::string & a_json, 
		ISerializable * a_pObject = NULL );
	//! deserialize object from json object
	static ISerializable * DeserializeObject( const Json::Value & a_json,
		ISerializable * a_pObject = NULL );
	//! serialize a object into json object, if a_bWriteType is true we store the object type into the JSON for deserialization uses.
	static Json::Value SerializeObject( ISerializable * a_pObject, 
		bool a_bWriteType = true );
	//! deserialize a object from a file containing json data.
	static ISerializable * DeserializeFromFile( const std::string & a_File, 
		ISerializable * a_pObject = NULL );
	//! serialize a object into a file 
	static bool SerializeToFile( const std::string & a_File, ISerializable * a_pObject, 
		 bool a_bWriteType = true, bool a_bFormatJson = false );


	//! Load object from file as template type.
	template<typename T>
	static T * DeserializeFromFile( const std::string & a_File, 
		ISerializable * a_pObject = NULL )
	{
		ISerializable * pUncasted = DeserializeFromFile( a_File, a_pObject );
		T * pCasted = DynamicCast<T>( pUncasted );
		if ( pCasted != NULL )
			return pCasted;
		if ( a_pObject != pUncasted )
			delete pUncasted;
		return NULL;
	}

	//! Helper to serialize an object into a specific pointer type, will destroy the created object
	//! if the DyanmicCast fails.
	template<typename T>
	static T * DeserializeObject( const Json::Value & a_json, ISerializable * a_pObject = NULL )
	{
		ISerializable * pUncasted = DeserializeObject( a_json, a_pObject );
		T * pCasted = DynamicCast<T>( pUncasted );
		if ( pCasted != NULL )
			return pCasted;
		if ( a_pObject != pUncasted )
			delete pUncasted;
		return NULL;
	}

	template<typename T>
	static T * DeserializeObject( const std::string & a_json, ISerializable * a_pObject = NULL )
	{
		ISerializable * pUncasted = DeserializeObject( a_json, a_pObject );
		T * pCasted = DynamicCast<T>( pUncasted );
		if ( pCasted != NULL )
			return pCasted;
		if ( a_pObject != pUncasted )
			delete pUncasted;
		return NULL;
	}

	//! Helper class for making serialized objects by ID
	template<typename T>
	static T * CreateObject( const std::string & a_Id )
	{
		ISerializable * pUncasted = GetSerializableFactory().CreateObject( a_Id );
		if ( pUncasted != NULL )
		{
			T * pCasted = DynamicCast<T>( pUncasted );
			if ( pCasted != NULL )
				return pCasted;

			// wrong object type, delete the created object..
			delete pUncasted;
		}

		return NULL;
	}

	//! Serialize the vector into the json
	template<typename T>
	static void SerializeVector( const char * a_Id, std::vector<T> & a_Vector, Json::Value & json )
	{
		for(size_t i=0;i<a_Vector.size();++i)
			json[a_Id].append(ISerializable::SerializeObject( &a_Vector[i], false ));
	}

	static void SerializeVector( const char * a_Id, std::vector<std::string> & a_Vector, Json::Value & json )
	{
		for(size_t i=0;i<a_Vector.size();++i)
			json[a_Id][i] = a_Vector[i];
	}

	static void SerializeVector(const char * a_Id, std::vector<float> & a_Vector, Json::Value & json)
	{
		for (size_t i = 0; i < a_Vector.size(); ++i)
			json[a_Id][i] = a_Vector[i];
	}

	template<typename T>
	static void SerializeVector( const char * a_Id, std::vector< boost::shared_ptr<T> > & a_Vector, Json::Value & json, bool a_bWriteType = true )
	{
		for(size_t i=0;i<a_Vector.size();++i)
			json[a_Id].append(ISerializable::SerializeObject( a_Vector[i].get(), a_bWriteType ));
	}

	template<typename K,typename T>
	static void SerializeMap( const char * a_Id, std::map< K, T > & a_Map, Json::Value & json )
	{
		int index = 0;
		for( typename std::map<K,T>::const_iterator iElement = a_Map.begin(); iElement != a_Map.end(); ++iElement )
		{
			Json::Value & element = json[a_Id][index++];
			element["key"] = iElement->first;
			element["value"] = ISerializable::SerializeObject( &iElement->second, false );
		}
	}

	template<typename K>
	static void SerializeMap( const char * a_Id, std::map< K, std::string > & a_Map, Json::Value & json )
	{
		int index = 0;
		for( typename std::map<K,std::string>::const_iterator iElement = a_Map.begin(); iElement != a_Map.end(); ++iElement )
		{
			Json::Value & element = json[a_Id][index++];
			element["key"] = iElement->first;
			element["value"] = iElement->second;
		}
	}
	template<typename K,typename T>
	static void SerializeMap( const char * a_Id, std::map< K, boost::shared_ptr<T> > & a_Map, Json::Value & json, bool a_bWriteType = true )
	{
		int index = 0;
		for( typename std::map< K, boost::shared_ptr<T> >::const_iterator iElement = a_Map.begin(); iElement != a_Map.end(); ++iElement )
		{
			Json::Value & element = json[a_Id][index++];
			element["key"] = iElement->first;
			element["value"] = ISerializable::SerializeObject( iElement->second.get(), a_bWriteType );
		}
	}

	template<typename T>
	static void SerializeList( const char * a_Id, std::list< boost::shared_ptr<T> > & a_List, Json::Value & json )
	{
		for( typename std::list< boost::shared_ptr<T> >::const_iterator iElement = a_List.begin(); iElement != a_List.end(); ++iElement )
			json[a_Id].append(ISerializable::SerializeObject( (*iElement).get() ));
	}

	static void SerializeList(const char * a_Id, std::list<std::string> & a_List, Json::Value & json)
	{
		for (std::list<std::string>::const_iterator iElement = a_List.begin(); iElement != a_List.end(); ++iElement)
			json[a_Id].append(*iElement);
	}

	//! Load the JSON into the given vector..
	template<typename T>
	static void DeserializeVector( const char * a_Id, const Json::Value & json, std::vector<T> & a_Vector )
	{
		a_Vector.clear();

		const Json::Value & elements = json[a_Id];
		for( Json::ValueConstIterator iElement = elements.begin(); iElement != elements.end(); ++iElement )
		{
			a_Vector.push_back( T() );
			ISerializable::DeserializeObject( *iElement, &a_Vector.back() );
		}
	}

	static void DeserializeVector( const char * a_Id, const Json::Value & json, std::vector<std::string> & a_Vector )
	{
		a_Vector.clear();

		const Json::Value & elements = json[a_Id];
		for(size_t i=0;i<elements.size();++i)
			a_Vector.push_back( elements[i].asString() );
	}

	static void DeserializeVector(const char * a_Id, const Json::Value & json, std::vector<float> & a_Vector)
	{
		a_Vector.clear();

		const Json::Value & elements = json[a_Id];
		for (size_t i = 0; i < elements.size(); ++i)
			a_Vector.push_back(elements[i].asFloat());
	}

	template<typename T>
	static void DeserializeVector( const char * a_Id, const Json::Value & json, std::vector< boost::shared_ptr<T> > & a_Vector)
	{
		a_Vector.clear();

		const Json::Value & elements = json[a_Id];
		for(size_t i=0;i<elements.size();++i)
			a_Vector.push_back( boost::shared_ptr<T>( ISerializable::DeserializeObject<T>( elements[i] ) ) );
	}

	template<typename T>
	static void DeserializeVectorNoType( const char * a_Id, const Json::Value & json, std::vector< boost::shared_ptr<T> > & a_Vector )
	{
		a_Vector.clear();

		const Json::Value & elements = json[a_Id];
		for(size_t i=0;i<elements.size();++i)
			a_Vector.push_back( boost::shared_ptr<T>( ISerializable::DeserializeObject<T>( elements[i], new T() ) ) );
	}

	template<typename K,typename T>
	static void DeserializeMap( const char * a_Id, const Json::Value & json, std::map<K,T> & a_Map, bool a_bClearMap = true )
	{
		if ( a_bClearMap )
			a_Map.clear();

		const Json::Value & elements = json[a_Id];
		for(size_t i=0;i<elements.size();++i)
		{
			const std::string & key = elements[i]["key"].asString();
			ISerializable::DeserializeObject( elements[i]["value"], &a_Map[key] );
		}
	}

	template<typename K>
	static void DeserializeMap( const char * a_Id, const Json::Value & json, std::map<K,std::string> & a_Map,
		bool a_bClearMap = true )
	{
		if ( a_bClearMap )
			a_Map.clear();

		const Json::Value & elements = json[a_Id];
		for(size_t i=0;i<elements.size();++i)
		{
			const std::string & key = elements[i]["key"].asString();
			a_Map[key] = elements[i]["value"].asString();
		}
	}

	template<typename K,typename T>
	static void DeserializeMap( const char * a_Id, const Json::Value & json, std::map<K, boost::shared_ptr<T> > & a_Map, 
		bool a_bClearMap = true )
	{
		if ( a_bClearMap )
			a_Map.clear();

		const Json::Value & elements = json[a_Id];
		for(size_t i=0;i<elements.size();++i)
		{
			const std::string & key = elements[i]["key"].asString();
			a_Map[key] = boost::shared_ptr<T>( ISerializable::DeserializeObject<T>( elements[i]["value"] ) );
		}
	}

	template<typename T>
	static void DeserializeList( const char * a_Id, const Json::Value & json, std::list< boost::shared_ptr<T> > & a_List )
	{
		a_List.clear();

		const Json::Value & elements = json[a_Id];
		for(size_t i=0;i<elements.size();++i)
			a_List.push_back( boost::shared_ptr<T>( ISerializable::DeserializeObject<T>( elements[i] ) ) );
	}

	static void DeserializeList(const char * a_Id, const Json::Value & json, std::list<std::string> & a_List)
	{
		a_List.clear();

		const Json::Value & elements = json[a_Id];
		for (size_t i = 0; i<elements.size(); ++i)
			a_List.push_back(elements[i].asString());
	}
};

//! This macro should be declared in the source file for any class that can be serialized by ISerializable
//! DO NOT place this macro in a header as it will create a compile error because of conflicting variable names.
#define REG_SERIALIZABLE( CLASS )						RegisterWithFactory<CLASS> serializable_##CLASS( #CLASS, ISerializable::GetSerializableFactory() );
#define REG_SERIALIZABLE_NESTED( PARENT, CLASS )		RegisterWithFactory<PARENT::CLASS> serializable_##PARENT_##CLASS( #CLASS, ISerializable::GetSerializableFactory() );
#define REG_SERIALIZABLE_VAR( VAR, CLASS)				RegisterWithFactory<CLASS> VAR( #CLASS, ISerializable::GetSerializableFactory() );
#define REG_OVERRIDE_SERIALIZABLE( OVERRIDE, CLASS )	RegisterWithFactory<CLASS> serializable_override_##CLASS( #OVERRIDE, ISerializable::GetSerializableFactory(), true );

#endif

