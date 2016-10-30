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

#ifndef SELF_NAMEINTENT_H
#define SELF_NAMEINTENT_H

#include "blackboard/IIntent.h"

class NameIntent : public IIntent
{
public:
    RTTI_DECL();

//! Types
    typedef boost::shared_ptr <NameIntent> SP;
    typedef boost::weak_ptr <NameIntent> WP;

    NameIntent();

    //! ISerializable interface
    virtual void Serialize(Json::Value &json);
    virtual void Deserialize(const Json::Value &json);
    //! IIntent interface
    virtual void Create(const Json::Value &a_Intent, const Json::Value &a_Parse);

    //! Accessors and Mutators
    const std::string & GetText() const
    {
        return m_Text;
    }

    const std::string & GetTarget() const
    {
        return m_Target;
    }

    const std::string & GetName() const
    {
        return m_Name;
    }

private:

    //! Data
    std::string m_Text;
    std::string m_Target;
    std::string m_Name;
};


#endif //SELF_NAMEINTENT_H
