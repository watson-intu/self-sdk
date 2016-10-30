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

#ifndef SELF_WEBREQUEST_H
#define SELF_WEBREQUEST_H

#include "IThing.h"
#include "utils/IWebClient.h"

//! This object is placed on the blackboard when we need self to make a rest request
class SELF_API WebRequest : public IThing
{
public:
    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr<WebRequest>			SP;
    typedef boost::weak_ptr<WebRequest>			WP;
    typedef IWebClient::Headers		Headers;

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! Construction
    WebRequest() : IThing( TT_COGNITIVE )
    {}

    WebRequest( const std::string & a_URL,
                const std::string & a_Type,
                const std::string & a_Body,
                const std::string & a_Params,
                const Headers & a_Headers) :
            IThing( TT_COGNITIVE ),
            m_URL ( a_URL ),
            m_Type ( a_Type ),
            m_Body ( a_Body ),
            m_Params ( a_Params ),
			m_Headers ( a_Headers )
	{}

    //! Accessors
    const std::string & GetParams() const
    {
        return m_Params;
    }

    void SetParams( const std::string & a_Params )
    {
        m_Params = a_Params;
    }

    const std::string & GetURL() const
    {
        return m_URL;
    }

    void SetURL( const std::string & a_URL )
    {
        m_URL = a_URL;
    }

    const std::string & GetType() const
    {
        return m_Type;
    }

    void SetType( const std::string & a_Type )
    {
        m_Type = a_Type;
    }

    const std::string & GetBody() const
    {
        return m_Body;
    }

    void SetBody( const std::string & a_Body )
    {
        m_Body = a_Body;
    }

    const Headers & GetHeaders() const
    {
        return m_Headers;
    }

    void SetHeaders( const Headers & a_Headers)
    {
        m_Headers = a_Headers;
    }

private:

    //! Data
    std::string         m_Params;
    std::string         m_URL;
    std::string         m_Type;
    std::string         m_Body;
    Headers             m_Headers;
};

#endif //SELF_WEBREQUEST_H
