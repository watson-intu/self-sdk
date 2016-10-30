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

#ifndef SELF_QUESTIONINTENT_H
#define SELF_QUESTIONINTENT_H

#include "IIntent.h"

class SELF_API QuestionIntent : public IIntent
{
public:
    RTTI_DECL();

	//! Types
	typedef boost::shared_ptr<QuestionIntent>		SP;
	typedef boost::weak_ptr<QuestionIntent>			WP;

	//! Construction
	QuestionIntent() : m_bLocalDialog( false )
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

    void SetPipeline(const std::string & a_Pipeline)
    {
        m_Pipeline = a_Pipeline;
    }

    const std::string & GetPipeline() const
    {
        return m_Pipeline;
    }
 
    void SetGoalParams(const Json::Value & a_GoalParams)
    {
        m_GoalParams = a_GoalParams;
    }
    
    const Json::Value & GetGoalParams() const
    {
        return m_GoalParams;
    }

	void SetLocalDialog( bool a_bLocalDialog )
	{
		m_bLocalDialog = a_bLocalDialog;
	}
	
	bool IsLocalDialog() const
	{
		return m_bLocalDialog;
	}

private:
    //! Data
    std::string m_Text;
    std::string m_Pipeline;
    Json::Value m_GoalParams;
	bool m_bLocalDialog;
};

#endif //SELF_QUESTIONINTENT_H
