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


#ifndef SELF_IMAGE_H
#define SELF_IMAGE_H

#include "IThing.h"

//! An image object taken from camera sensor
class SELF_API Image : public IThing
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Image>			SP;
	typedef boost::weak_ptr<Image>				WP;

	//! Construction
	Image() : IThing( TT_PERCEPTION, 30.0f)
	{}

	//! Accessors
    const std::string & GetContent() const
    {
        return m_Content;
    }
    void SetContent( const std::string & a_Content )
    {
        m_Content = a_Content;
    }

    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

private:
    //! Data
    std::string m_Content;

};

#endif //SELF_IMAGE_H
