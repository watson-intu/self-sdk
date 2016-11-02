//
// Created by John Andersen on 11/2/16.
//

#ifndef SELF_WORKSHOPFIVEGESTURE_H
#define SELF_WORKSHOPFIVEGESTURE_H

#include "gestures/AnimateGesture.h"

//! This is the class for animating a Raspberry Pi, such as changing LED colors.
class WorkshopFiveGesture : public AnimateGesture
{
public:
    RTTI_DECL();

    //! IGesture interface
    virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
    virtual bool Abort();

    //! Construction
    WorkshopFiveGesture() : m_PinNumber( 7 ), m_bWiredPi( false )
    {}

private:
    //! Data
    bool m_bWiredPi;
    int m_PinNumber;

    //! Callbacks
    void AnimateThread( Request * a_pReq );
    void DoAnimateThread( Request * a_pReq );
    void AnimateDone( Request * a_pReq );
};


#endif //SELF_WORKSHOPFIVEGESTURE_H
