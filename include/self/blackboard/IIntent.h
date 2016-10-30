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

#ifndef SELF_IINTENT_H
#define SELF_IINTENT_H

#include "blackboard/IThing.h"
#include "utils/RTTI.h"

class SELF_API IIntent : public IThing
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<IIntent>		SP;
	typedef boost::weak_ptr<IIntent>		WP;

    //! Construction
	IIntent( float a_fCondidence = 0.0f ) : m_Confidence(a_fCondidence), IThing( TT_PERCEPTION )
	{}
    virtual ~IIntent()
    {}

    //! ISerializable
    virtual void Serialize(Json::Value & json)
    {
		IThing::Serialize( json );
        json["m_Confidence"] = m_Confidence;
    }
    virtual void Deserialize(const Json::Value & json)
    {
		IThing::Deserialize( json );
        if( json.isMember("m_Confidence") )
            m_Confidence = json["m_Confidence"].asDouble();
    }

	double GetConfidence() const { return m_Confidence; }
	void SetConfidence( double a_Confidence ) { m_Confidence = a_Confidence; }

	virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse) = 0;
protected:
    double  m_Confidence;
};

#endif //SELF_IINTENT_H
