//
// Created by John Andersen on 6/23/16.
//

#include "CameraIntent.h"
#include "utils/StringUtil.h"

RTTI_IMPL(CameraIntent, IIntent);
REG_SERIALIZABLE(CameraIntent);

CameraIntent::CameraIntent()
{}

void CameraIntent::Serialize(Json::Value &json)
{
    IIntent::Serialize(json);

}

void CameraIntent::Deserialize(const Json::Value &json)
{
    IIntent::Deserialize(json);

}

void CameraIntent::Create(const Json::Value & a_Intent, const Json::Value & a_Parse)
{
    std::vector<std::string> m_Words;
    StringUtil::Split(a_Intent["top_class"].asString(), "_",m_Words);
    m_Target = m_Words.at(1);
}

