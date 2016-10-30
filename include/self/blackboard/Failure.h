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

#ifndef SELF_FAILURE_H
#define SELF_FAILURE_H

#include "blackboard/IThing.h"
#include "utils/RTTI.h"

class SELF_API Failure : public IThing
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Failure>		SP;
	typedef boost::weak_ptr<Failure>		WP;

    //! Construction
	Failure() : IThing( TT_PERCEPTION ), m_Confidence( 0.0 ), m_Threshold( 0.0 )
	{}
	Failure( const std::string & a_Name, const std::string & a_Info, double a_Confidence, double a_Threshold ) : 
		IThing( TT_PERCEPTION ),
        m_Name(a_Name),
        m_Info(a_Info),
        m_Confidence(a_Confidence),
        m_Threshold(a_Threshold) 
	{}

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

private:
    std::string     m_Name;
    std::string     m_Info;
    double          m_Confidence;
	double          m_Threshold;
};

#endif //SELF_FAILURE_H
