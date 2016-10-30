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

#ifndef SELF_REQUESTINTENT_H
#define SELF_REQUESTINTENT_H

#include "IIntent.h"

class SELF_API RequestIntent : public IIntent
{
public:
    RTTI_DECL();

    //! Types
	typedef boost::shared_ptr<RequestIntent>	SP;
	typedef boost::weak_ptr<RequestIntent>		WP;

	struct Request
	{
		Request()
		{}
		Request(const std::string & a_verb, const std::string & a_target) :
			m_Verb(a_verb), m_Target(a_target)
		{}
		std::string		m_Verb;
		std::string		m_Target;
	};
	typedef std::vector< Request >				RequestList;

	RequestIntent()
	{}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

	//! IIntent interface
	virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

    //! Accessors
	const std::string & GetText() const
	{
        return m_Text;
    }

	const Json::Value & GetTags() const		
	{
		return m_Tags;
	}

	const std::string & GetType() const
	{
        return m_Type;
    }

	// NOTE: This will probably get removed eventually in favor of the GetTextParse() which is a 
	// more complete representation of what was requested.
    const RequestList & GetRequests() const
	{
        return m_Requests;
    }

private:
    //! Data
    std::string m_Text;
	Json::Value m_Tags;
    std::string m_Type;
	RequestList	m_Requests;
	Json::Value m_Parse;

};

#endif //SELF_REQUESTINTENT_H
