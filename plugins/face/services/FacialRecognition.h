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

#ifndef SELF_FACIALRECOGNITION_H
#define SELF_FACIALRECOGNITION_H

#include "services/IService.h"
#include "DataModels.h"

class FacialRecognition : public IService
{
public:

    RTTI_DECL();

    //! Types
    typedef Delegate<Identify *>	        OnIdentifyFaces;
    typedef Delegate<Detect *>              OnDetectFaces;
    typedef Delegate<Session *>             OnSession;
    typedef Delegate<Trainer *>             OnTrainFaces;
    typedef Delegate<People *>              OnPeople;
    typedef Delegate<const Json::Value &>   OnAddedPerson;
    typedef Delegate<const Json::Value &>   OnAddedFace;

    //! Construction
    FacialRecognition();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    //! Identify faces in the provided image
    void IdentifyFaces( const std::string & a_ImageData,
                        OnIdentifyFaces a_Callback );
    void AddPerson( const std::string & a_Person,
                        OnAddedPerson a_Callback );
    void AddFace( const std::string & a_Person, const std::string & a_FaceId,
                        OnAddedFace a_Callback );
    void DetectFace( const std::string & a_ImageData,
                        OnDetectFaces a_Callback );
    void TrainIdentifier( OnTrainFaces a_Callback );
    void GetSessionStatus (const std::string & a_SessionId, OnSession a_Callback );
    void GetAllPeople( OnPeople a_Callback );

private:

    //! Data
    std::string m_Key;
    std::string m_Secret;
    std::string m_Group;

};
#endif //SELF_FACIALRECOGNITION_H
