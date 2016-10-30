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

#include "FacialRecognition.h"
#include "utils/Form.h"
#include "SelfInstance.h"


REG_SERIALIZABLE(FacialRecognition);
RTTI_IMPL( FacialRecognition, IService );
RTTI_IMPL(Trainer,ISerializable);
RTTI_IMPL(Session,ISerializable);
RTTI_IMPL(Identify,ISerializable);
RTTI_IMPL(Detect,ISerializable);
RTTI_IMPL(Face,ISerializable);
RTTI_IMPL(People,ISerializable);

FacialRecognition::FacialRecognition() : IService("FacialRecognition")
{}

void FacialRecognition::Serialize(Json::Value & json)
{
    IService::Serialize(json);

    json["m_Key"] = m_Key;
    json["m_Secret"] = m_Secret;
    json["m_Group"] = m_Group;
}

void FacialRecognition::Deserialize(const Json::Value & json)
{
    IService::Deserialize(json);

    m_Key = json["m_Key"].asString();
    m_Secret = json["m_Secret"].asString();
    m_Group = json["m_Group"].asString();
}

bool FacialRecognition::Start()
{
    Log::Debug("FacialRecognition", "Starting FacialRecognition!");
    if (!IService::Start())
        return false;

    SelfInstance *pInstance = SelfInstance::GetInstance();
    if (pInstance == NULL) {
        Log::Error("FacialRecognition", "SelfInstance is NULL.");
        return false;
    }
    return true;
}

void FacialRecognition::IdentifyFaces( const std::string & a_ImageData, OnIdentifyFaces a_Callback )
{
    std::string parameters = "/v2/recognition/identify";
    parameters += "?api_secret=" + m_Secret;
    parameters += "&api_key=" + m_Key;
    parameters += "&group_name=" + m_Group;

    Form form;
    form.AddFilePart("img", "AlchemyTest.jpg", a_ImageData);
    form.Finish();

    Headers headers;
    headers["Content-Type"] = form.GetContentType();

    new RequestObj<Identify>(this, parameters, "POST", headers, form.GetBody(), a_Callback);
}

void FacialRecognition::AddPerson( const std::string & a_Person, OnAddedPerson a_Callback )
{
    std::string parameters = "/v2/person/create";
    parameters += "?api_secret=" + m_Secret;
    parameters += "&api_key=" + m_Key;
    parameters += "&person_name=" + a_Person;
    parameters += "&group_name=" + m_Group;

    new RequestJson(this, parameters, "GET", NULL_HEADERS, EMPTY_STRING, a_Callback);
}

void FacialRecognition::AddFace( const std::string & a_Person, const std::string & a_FaceId, OnAddedFace a_Callback )
{
    std::string parameters = "/v2/person/add_face";
    parameters += "?api_secret=" + m_Secret;
    parameters += "&api_key=" + m_Key;
    parameters += "&person_name=" + a_Person;
    parameters += "&face_id=" + a_FaceId;

    new RequestJson(this, parameters, "GET", NULL_HEADERS, EMPTY_STRING, a_Callback);
}

void FacialRecognition::DetectFace( const std::string & a_ImageData, OnDetectFaces a_Callback )
{
    std::string parameters = "/v2/detection/detect";
    parameters += "?api_secret=" + m_Secret;
    parameters += "&api_key=" + m_Key;

    Form form;
    form.AddFilePart("img", "test.jpg", a_ImageData);
    form.Finish();

    Headers headers;
    headers["Content-Type"] = form.GetContentType();

    new RequestObj<Detect>(this, parameters, "POST", headers, form.GetBody(), a_Callback);
}

void FacialRecognition::TrainIdentifier( OnTrainFaces a_Callback )
{
    std::string parameters = "/v2/train/identify";
    parameters += "?api_secret=" + m_Secret;
    parameters += "&api_key=" + m_Key;
    parameters += "&group_name=" + m_Group;

    new RequestObj<Trainer>(this, parameters, "GET", NULL_HEADERS, EMPTY_STRING, a_Callback);
}

void FacialRecognition::GetSessionStatus( const std::string & a_SessionId, OnSession a_Callback )
{
    std::string parameters = "/v2/info/get_session";
    parameters += "?api_secret=" + m_Secret;
    parameters += "&api_key=" + m_Key;
    parameters += "&session_id=" + a_SessionId;

    new RequestObj<Session>(this, parameters, "GET", NULL_HEADERS, EMPTY_STRING, a_Callback);
}

void FacialRecognition::GetAllPeople( OnPeople a_Callback )
{
    std::string parameters = "/v2/info/get_person_list";
    parameters += "?api_secret=" + m_Secret;
    parameters += "&api_key=" + m_Key;

    new RequestObj<People>(this, parameters, "GET", NULL_HEADERS, EMPTY_STRING, a_Callback);
}



