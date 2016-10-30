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

#ifndef SELF_NLC_PROXY_H
#define SELF_NLC_PROXY_H

#include "ITextClassifierProxy.h"
#include "blackboard/Text.h"

class NaturalLanguageClassifier;
struct Classifiers;
struct Classifier;

class SELF_API NLCProxy : public ITextClassifierProxy
{
public:
    RTTI_DECL();

	//! Construction
	NLCProxy();
	virtual ~NLCProxy();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

    //! ITextClassifierProxy interface
	virtual void Start();
	virtual void Stop();

	virtual void ClassifyText( Text::SP a_spText, Delegate<ClassifyResult *> a_Callback );
    
	//! Types

	//! Classifier training and management functions
	const std::string & GetClassifierId() const
	{
		return m_ClassifierId;
	}

	template<typename T>
	boost::shared_ptr<T> FindFilter() const
	{
		for (size_t i = 0; i < m_Filters.size(); ++i)
		{
			boost::shared_ptr<T> spFilter = DynamicCast<T>( m_Filters[i] );
			if (spFilter)
				return spFilter;
		}
		return boost::shared_ptr<T>();
	}

	// queue data to train our classifier with a new phrase and class.
	bool RetrainClassifier(const std::string & a_Text, const std::string & a_Class);
	void CheckClassifiers();
	void OnGetClassifiers( Classifiers * a_pClassifiers );
	void OnClassifierDeleted(const Json::Value & json);
	void OnClassifierTrained( Classifier * a_pClassifer );

private:
	//! Data
	NaturalLanguageClassifier *		m_pNLC;
	std::string						m_ClassifierFile;				// training file to upload/update
	std::string						m_ClassifierId;					// intent classifier to use
	std::string						m_Language;
	bool							m_bCheckedClassifiers;			// true once we've checked our classifiers

	TimerPool::ITimer::SP			m_spClassifiersTimer;			// timer use to check our classifiers

	//! Helper request object
	class Request
	{
    public:
        Request( NLCProxy * a_pNLCProxy, Text::SP a_spText, Delegate<ClassifyResult *> a_Callback );

		void OnTextClassified(const Json::Value & json);

    private:
        NLCProxy *              	        m_pProxy;
        Text::SP                         	m_spText;
        Delegate<ClassifyResult *>     		m_Callback;
	};
};

#endif // SELF_NLC_PROXY_H