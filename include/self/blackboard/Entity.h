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

#ifndef SELF_ENTITY_H
#define SELF_ENTITY_H

#include "IThing.h"

//! An entity object classified from Visual Recognition
class SELF_API Entity : public IThing
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Entity>		SP;
	typedef boost::weak_ptr<Entity>			WP;

	Entity() : IThing( TT_PERCEPTION )
	{}

    const std::string & GetTopClass() const
    {
        return m_TopClass;
    }
    void SetTopClass( const std::string & a_TopClass )
    {
        m_TopClass = a_TopClass;
    }

    const double & GetTopScore() const
    {
        return m_TopScore;
    }

    void SetTopScore( const double & a_TopScore )
    {
        m_TopScore = a_TopScore;
    }

    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

private:
    //! Data
    std::string m_TopClass;
    double      m_TopScore;

};

#endif //SELF_ENTITY_H
