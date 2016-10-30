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

#ifndef SELF_LOCAL_SPEECH_TO_TEXT_H
#define SELF_LOCAL_SPEECH_TO_TEXT_H

#include <list>

#include "TextData.h"
#include "ISensor.h"
#include "SelfLib.h"

//! This is the base class for a speech to text sensor implementation, this is not a concrete implementation.
class SELF_API LocalSpeechToText : public ISensor
{
public:
    RTTI_DECL();

    //! Construction
    LocalSpeechToText()
    {}

    ////! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! ISensor interface
    virtual const char * GetSensorName()
    {
        return "SpeechToText";
    }
    virtual const char * GetDataType()
    {
        return TextData::GetStaticRTTI().GetName().c_str();
    }

    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

protected:
    //! Data
    float			            m_MinConfidence;
    std::vector<std::string>    m_VocabularyList;
};

#endif
