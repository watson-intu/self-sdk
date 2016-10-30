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

#ifndef SELF_WEXINTENT_H
#define SELF_WEXINTENT_H

#include "IIntent.h"

class SELF_API WEXIntent : public IIntent
{
public:
    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr<WEXIntent>		SP;
    typedef boost::weak_ptr<WEXIntent>			WP;

    //! Construction
    WEXIntent()
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

    void SetGoalParams(const Json::Value & a_GoalParams)
    {
        m_GoalParams = a_GoalParams;
    }

    const Json::Value & GetGoalParams() const
    {
        return m_GoalParams;
    }

private:
    //! Data
    std::string m_Text;
    std::string m_Collection;
    Json::Value m_GoalParams;
};

#endif //SELF_QUESTIONINTENT_H
