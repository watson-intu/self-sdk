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

#ifndef SELF_FR_DATAMODELS_H
#define SELF_FR_DATAMODELS_H

#include "utils/ISerializable.h"

struct Trainer : public ISerializable
{
    RTTI_DECL();

    std::string m_Session;

    virtual void Serialize(Json::Value & json)
    {
        json["session_id"] = m_Session;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if ( json.isMember("session_id") )
            m_Session = json["session_id"].asString();
    }
};

struct Session : public ISerializable
{
    RTTI_DECL();

    std::string m_Session;
    std::string m_Status;

    virtual void Serialize(Json::Value & json)
    {
        json["session_id"] = m_Session;
        json["status"] = m_Status;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if ( json.isMember("session_id") )
            m_Session = json["session_id"].asString();
        if ( json.isMember("status") )
            m_Status = json["status"].asString();
    }
};

struct Identify : public ISerializable
{
    RTTI_DECL();

    std::string     m_Name;
    std::string     m_Id;
    double          m_Confidence;

    virtual void Serialize(Json::Value & json)
    {
        json["face"][0]["candidate"][0]["person_name"] = m_Name;
        json["face"][0]["candidate"][0]["person_id"] = m_Id;
        json["face"][0]["candidate"][0]["confidence"] = m_Confidence;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("face") )
        {
            m_Name = json["face"][0]["candidate"][0]["person_name"].asString();
            m_Id = json["face"][0]["candidate"][0]["person_id"].asString();
            m_Confidence = json["face"][0]["candidate"][0]["confidence"].asDouble();
        }
    }
};

struct Detect : public ISerializable
{
    RTTI_DECL();

    std::string m_FaceId;

    virtual void Serialize(Json::Value & json)
    {
        json["face"][0]["face_id"] = m_FaceId;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("face") )
        {
            m_FaceId = json["face"][0]["face_id"].asString();
        }
    }
};

struct Face : public ISerializable
{
    RTTI_DECL();
    std::string     m_Name;
    std::string     m_Id;

    virtual void Serialize(Json::Value & json)
    {
        json["person_id"] = m_Id;
        json["person_name"] = m_Name;
    }

    virtual void Deserialize(const Json::Value & json)
    {
        if( json.isMember("person_id") )
        {
            m_Id = json["person_id"].asString();
            m_Name = json["person_name"].asString();
        }
    }
};

struct People : public ISerializable
{
    RTTI_DECL();
    std::vector<Face>     m_People;

    virtual void Serialize(Json::Value & json)
    {
        SerializeVector( "person", m_People, json );
    }

    virtual void Deserialize(const Json::Value & json)
    {
        DeserializeVector( "person", json, m_People );
    }
};


#endif //SELF_FR_DATAMODELS_H
