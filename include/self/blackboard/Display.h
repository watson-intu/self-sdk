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


#ifndef SELF_DISPLAY_H
#define SELF_DISPLAY_H

#include "IThing.h"

//! This object is placed on the blackboard when we need self to say anything
class SELF_API Display : public IThing
{
public:
    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr<Display>			SP;
    typedef boost::weak_ptr<Display>			WP;

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! Construction
    Display() : IThing( TT_COGNITIVE )
    {}

    Display(const std::string & a_DisplayType,
            const std::string & a_Data) :
            IThing( TT_COGNITIVE ),
            m_DisplayType ( a_DisplayType ),
            m_Data ( a_Data )
    {}

    //! Accessors
    const std::string & GetDisplay() const
    {
        return m_DisplayType;
    }

    void SetDisplay( const std::string & a_DisplayType )
    {
        m_DisplayType = a_DisplayType;
    }

    const std::string & GetData() const
    {
        return m_Data;
    }

    void SetData( const std::string & a_Data )
    {
        m_Data = a_Data;
    }

private:
    //! Data
    std::string m_DisplayType;
    std::string m_Data;
};

#endif //SELF_IMAGE_H
