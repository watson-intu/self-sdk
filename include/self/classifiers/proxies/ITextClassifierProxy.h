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

#ifndef SELF_ITEXTCLASSIFIER_PROXY_H
#define SELF_ITEXTCLASSIFIER_PROXY_H

#include "blackboard/Text.h"
#include "blackboard/IIntent.h"

class SELF_API ITextClassifierProxy : public ISerializable, 
	public boost::enable_shared_from_this<ITextClassifierProxy>
{
public:
    RTTI_DECL();

    //! Types
	typedef boost::shared_ptr<ITextClassifierProxy>		SP;
	typedef boost::weak_ptr<ITextClassifierProxy>		WP;

	//! This struct is passed back through the callback to the TextClassifier with the results of this proxy
	struct ClassifyResult
	{
		ClassifyResult() : m_fConfidence( 0.0 ), m_bPriority( false )
		{}

		double			m_fConfidence;			// the confidence in this classification
		std::string		m_TopClass;				// the top classification name
		Json::Value		m_Result;				// the actual result data
		bool			m_bPriority;			// true if this is priority result
	};

	//! Interface class for filtering NLC results before we generate an intent. 
	class SELF_API IClassFilter : public ISerializable
	{
	public:
		RTTI_DECL();

		//! Types
		typedef boost::shared_ptr<IClassFilter>		SP;
		typedef boost::weak_ptr<IClassFilter>		WP;

		//! Interface
		virtual bool ApplyFilter(Json::Value & a_Intent) = 0;		// return true if classification should be ignored
	};
	typedef std::vector<IClassFilter::SP>		Filters;

	//! Construction
	ITextClassifierProxy() : m_bPriority( false )
	{}
	virtual ~ITextClassifierProxy()
	{}

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! Interface
	virtual void Start() = 0;
	virtual void Stop() = 0;

	virtual void ClassifyText( Text::SP a_spText, Delegate<ClassifyResult *> a_Callback ) = 0;

	//! Accessors
	bool HasPriority() const { return m_bPriority; }

protected:
	//! Data
	bool			m_bPriority;
	Filters			m_Filters;
};

#endif // SELF_ITEXTCLASSIFIER_PROXY_H