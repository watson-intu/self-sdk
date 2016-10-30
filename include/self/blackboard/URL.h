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

#ifndef SELF_URL_H
#define SELF_URL_H

#include "IThing.h"

//! This is the URL object that gets put onto the blackboard
class SELF_API Url : public IThing
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Url>		SP;
	typedef boost::weak_ptr<Url>		WP;

    //! Construction
	Url() : IThing( TT_COGNITIVE )
	{}
    Url( const std::string & a_URL ) : IThing( TT_COGNITIVE ), m_URL ( a_URL )
    {}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Accessors and mutators
    const std::string & GetURL() const { return m_URL; }
    void SetURL( const std::string & a_URL ) { m_URL = a_URL; }

private:
    //! Data
    std::string         m_URL;
};

#endif // URL
