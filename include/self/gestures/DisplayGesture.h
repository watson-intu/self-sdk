/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#ifndef SELF_DISPLAYGESTURE_H
#define SELF_DISPLAYGESTURE_H

#include "IGesture.h"
#include "SelfLib.h"

//! This gesture can be used to display a visualization on a screen.
class SELF_API DisplayGesture : public IGesture
{
public:
    RTTI_DECL();

    //! Construction
    DisplayGesture()
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IGesture interface
    virtual bool CanExecute( const ParamsMap & a_Params );
    virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
    virtual bool Abort();
};

#endif //SELF_DISPLAYGESTURE_H