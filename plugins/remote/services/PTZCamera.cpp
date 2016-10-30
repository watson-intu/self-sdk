//
// Created by John Andersen on 6/23/16.
//

#include "PTZCamera.h"
#include "SelfInstance.h"

REG_SERIALIZABLE(PTZCamera);
RTTI_IMPL(PTZCamera, IService);

PTZCamera::PTZCamera() : IService("PTZCamera")
{}

void PTZCamera::Serialize(Json::Value & json)
{
    IService::Serialize(json);
}


void PTZCamera::Deserialize(const Json::Value & json)
{
    IService::Deserialize(json);

}

bool PTZCamera::Start()
{
    Log::Debug("PTZCamera", "Starting PTZCamera!");
    if (!IService::Start())
        return false;

    return true;
}

IService::Request * PTZCamera::GetImage(GetImageObject a_Callback)
{
    Log::Debug("PTZCamera", "Invoking Get Image Request");
    std::string parameters = "/jpg/image.jpg?size=3";

    return new RequestData(this, parameters, "GET", NULL_HEADERS, EMPTY_STRING, a_Callback);
}

IService::Request * PTZCamera::SetCameraCoordinates(const std::string & a_Direction, GetImageObject a_Callback)
{
    Log::Debug("PTZCamera", "Invoking Set Camera Request");
    std::string parameters = StringUtil::Format("/axis-cgi/com/ptz.cgi?move=%s&camera=1", a_Direction.c_str());

   return new RequestData(this, parameters, "GET", NULL_HEADERS, EMPTY_STRING, a_Callback);
}


