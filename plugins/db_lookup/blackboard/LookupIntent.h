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

#ifndef SELF_LOOKUP_INTENT_H
#define SELF_LOOKUP_INTENT_H

#include "blackboard/IIntent.h"

class LookupIntent : public IIntent
{
public:
    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr<LookupIntent>			SP;
    typedef boost::weak_ptr<LookupIntent>			WP;

    //! Construction
    LookupIntent()
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IIntent interface
    virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

    void SetText(const std::string & a_Text)
    {
        m_Text = a_Text;
    }

    const std::string & GetText() const
    {
        return m_Text;
    }

    void SetCollection(const std::string & a_Collection)
    {
        m_Collection = a_Collection;
    }

    const std::string & GetCollection() const
    {
        return m_Collection;
    }

    void SetEntity(const std::string & a_Entity)
    {
        m_Entity = a_Entity;
    }

    const std::string & GetEntity() const
    {
        return m_Entity;
    }

    void SetIntent(const std::string & a_Intent)
    {
        m_Intent = a_Intent;
    }

    const std::string & GetIntent() const
    {
        return m_Intent;
    }


private:
    //! Data
    std::string m_Text;
    std::string m_Collection;
    std::string m_Entity;
    std::string m_Intent;
};

#endif //SELF_LOOKUP_INTENT_H
