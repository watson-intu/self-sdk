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

#ifndef SELF_TEXT_H
#define SELF_TEXT_H

#include "IThing.h"
#include "utils/StringUtil.h"

//! Recognized speech or entered text, usually attached to a person object if they are known. This object
//! will send the get processed by the TextClassifier object and a intent object will be attached to it on the 
//! blackboard.

//! NOTE: Language is set the original language before it has been translated into the back-end language
//! The DialogAgent will use this to know what to translate the language back into when responding to the user.
class SELF_API Text : public IThing
{
public:
	RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<Text>		SP;
	typedef boost::weak_ptr<Text>		WP;

	//! Construction
	Text() : IThing( TT_PERCEPTION ), m_LocalDialog( false ), m_ClassifyIntent( true ), m_fConfidence( 1.0 )
	{}
	Text( const std::string & a_Speech, double a_fConfidence, bool a_bLocalDialog, bool a_bClassifyIntent, const std::string & a_Language = "en-US" ) : 
		IThing( TT_PERCEPTION ),
		m_Text ( StringUtil::Trim(a_Speech) ),
		m_fConfidence( a_fConfidence ),
		m_LocalDialog ( a_bLocalDialog ),
		m_ClassifyIntent( a_bClassifyIntent ),
		m_Language( a_Language )
	{}

	const std::string & GetText() const
	{
		return m_Text;
	}
	double GetConfidence() const
	{
		return m_fConfidence;
	}
	bool IsLocalDialog() const
	{
		return m_LocalDialog;
	}
	bool ClassifyIntent() const 
	{
		return m_ClassifyIntent;
	}
	const std::string & GetLanguage() const
	{
		return m_Language;
	}

	void SetText(const std::string & a_Text)
	{
		m_Text = a_Text;
	}

	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

private:
	//! Data
	std::string m_Text;
	double		m_fConfidence;
	bool 		m_LocalDialog;
	bool		m_ClassifyIntent;
	std::string m_Language;
};

#endif
