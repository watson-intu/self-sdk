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


#ifndef SELF_ATTENTION_H
#define SELF_ATTENTION_H

#include "IThing.h"

//! This object is placed on the blackboard when we need self to say anything
class SELF_API Attention : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Attention>			SP;
	typedef boost::weak_ptr<Attention>			WP;

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Construction
	Attention() : IThing(TT_PERCEPTION, 30.0f)
	{}
	Attention(double a_MinIntentConfidence, double a_MinMissNodeConfidence) :
		m_MinIntentConfidence(a_MinIntentConfidence), m_MinMissNodeConfidence(a_MinMissNodeConfidence), IThing(TT_PERCEPTION, 30.0f)
	{}

	//! Accessors
	double GetMinIntentConfidence() const
	{
		return m_MinIntentConfidence;
	}

	void SetMinIntentConfidence(double a_MinIntentConfidence)
	{
		m_MinIntentConfidence = a_MinIntentConfidence;
	}

	double GetMinMissNodeConfidence() const
	{
		return m_MinMissNodeConfidence;
	}

	void SetMinMissNodeConfidence(double a_MinMissNodeConfidence)
	{
		m_MinMissNodeConfidence = a_MinMissNodeConfidence;
	}

private:
	//! Data
	double m_MinIntentConfidence;
	double m_MinMissNodeConfidence;
};

#endif //SELF_ATTENTION_H

