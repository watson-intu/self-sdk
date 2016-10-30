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

#ifndef SELF_LEARNINGINTENT_H
#define SELF_LEARNINGINTENT_H

#include "IIntent.h"

class SELF_API LearningIntent : public IIntent
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<LearningIntent>		SP;
	typedef boost::weak_ptr<LearningIntent>			WP;

	LearningIntent()
	{}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

	//! IIntent interface
	virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

	//! Accessors and Mutators
	const std::string & GetText() const
	{
        return m_Text;
    }

	const Json::Value & GetTags() const 
	{
		return m_Tags;
	}

	const std::string & GetTarget() const
	{
        return m_Target;
    }

	const std::string & GetVerb() const
	{
        return m_Verb;
    }

	const std::string & GetAnswer() const
	{
		return m_Answer;
	}

private:
    //! Data
    std::string m_Text;
	Json::Value m_Tags;
    std::string m_Target;
    std::string m_Verb;
	std::string m_Answer;
	Json::Value m_Parse;

	std::string FindTags( const std::string & a_Tag );
};

#endif //SELF_LEARNINGINTENT_H
