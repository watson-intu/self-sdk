//
// Created by John Andersen on 6/23/16.
//

#ifndef SELF_PTZCAMERA_H
#define SELF_PTZCAMERA_H

#include "services/IService.h"

class PTZCamera : public IService
{
public:

    RTTI_DECL();

    typedef Delegate<const std::string &> GetImageObject;

    //! Construction
    PTZCamera();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    IService::Request * GetImage(GetImageObject a_Callback);
    IService::Request * SetCameraCoordinates(const std::string & a_Direction, GetImageObject a_Callback);

private:


};

#endif //SELF_PTZCAMERA_H
