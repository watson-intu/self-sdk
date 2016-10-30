//
// Created by John Andersen on 6/23/16.
//

#ifndef SELF_CAMERAINTENT_H
#define SELF_CAMERAINTENT_H

#include "blackboard/IIntent.h"

class CameraIntent : public IIntent
{
public:

    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr <CameraIntent> SP;
    typedef boost::weak_ptr <CameraIntent> WP;

    CameraIntent();

    //! ISerializable interface
    virtual void Serialize(Json::Value &json);

    virtual void Deserialize(const Json::Value &json);

    //! IIntent interface
    virtual void Create(const Json::Value &a_Intent, const Json::Value &a_Parse);

    //! Accessors and Mutators
    const std::string &GetText() const {
        return m_Text;
    }

    const std::string &GetTarget() const {
        return m_Target;
    }

    const std::string &GetName() const {
        return m_Name;
    }

private:

    //! Data
    std::string m_Text;
    std::string m_Target;
    std::string m_Name;
};

#endif //SELF_CAMERAINTENT_H
