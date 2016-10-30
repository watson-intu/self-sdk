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

#ifndef SELF_TEXT_EXTRACTOR_H
#define SELF_TEXT_EXTRACTOR_H

#include <list>
#include <deque>

#include "IFeatureExtractor.h"
#include "blackboard/ThingEvent.h"
#include "sensors/SensorManager.h"
#include "services/SpeechToText/SpeechToText.h"
#include "services/LanguageTranslation/LanguageTranslation.h"
#include "utils/Factory.h"
#include "blackboard/Text.h"

#include "SelfLib.h"

class SelfInstance;

class SELF_API TextExtractor : public IFeatureExtractor
{
public:
	RTTI_DECL();

	//! Interface class for filtering Audio before sending to STT 
	class SELF_API IAudioFilter : public ISerializable,
		public boost::enable_shared_from_this<IAudioFilter>
	{
	public:
		RTTI_DECL();

		//! Types
		typedef boost::shared_ptr<IAudioFilter>		SP;
		typedef boost::weak_ptr<IAudioFilter>		WP;
		//! Base class functions
		IAudioFilter();
		void SetActiveState(bool a_State);
		bool GetActiveState();
		void ApplyFilter(SpeechAudioData & a_Data);
		//! Interface
		virtual void DoApplyFilter(SpeechAudioData & a_Data) = 0;
	protected:
		//! Data
		bool		m_bActive;
		double		m_TimeProcessed;
		double		m_TimeInterval;
		double		m_TimeLast;
		double		m_Window;
	};

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

	//! Construction
	TextExtractor();
	~TextExtractor();

	//! ISerialziable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

    //! IFeatureExtractor interface
    virtual const char * GetName() const;
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    typedef SensorManager::SensorList			SensorList;
    typedef std::vector<IAudioFilter::SP>		Filters;

    //! Data
	float				m_MinConfidence;
	float 				m_MaxConfidence;
	float 				m_ConfidenceThreshold;
	float 				m_ConfidenceThresholdLocal;
	float				m_MinSpeechLevel;
	float 				m_StdDevThreshold;
	float 				m_EnergyTimeInterval;
	float 				m_NameRecognizedThresholdDrop;
	float 				m_MinEnergyAvg;
	float				m_MaxEnergyAvg;
	float 				m_NormalizedEnergyLevel; //Between 0 and 1
	double				m_MinFailureResponseInterval;
	int					m_EnergyAverageSampleCount;
	int 				m_MaxFailureResponsesCount;
	int 				m_BurnInCycles;
	int 				m_FailureResponsesCount;
	double 				m_EnergyLevelCrossedTimestamp;
	float 				m_AgeTimeout;
	int 				m_MinimumAge;
	bool 				m_bStoreAudio;
	std::vector<std::string>
						m_FailureResponses;
	std::deque<float>	m_EnergyLevelAverages;

	TimerPool::ITimer::SP 
						m_spAgeTimeout;

	double				m_LastFailureResponse;
    SensorList     		m_TextSensors;
	ISensor *			m_pAudioSensor;
	SpeechToText *		m_pSpeechToText;
	LanguageTranslation *
						m_pTranslation;
	bool 				m_Listening;

	Filters				m_Filters;

    //! Callback handler
	void OnAddAudio(ISensor * a_pSensor);
	void OnRemoveAudio(ISensor * a_pSensor);
    void OnAudioData(IData * data);
    void OnTextData(IData * data);
	void OnHealth( const ThingEvent & a_Event );
	void OnConversation(const ITopics::Payload & );
    void OnRecognizeSpeech(RecognizeResults * a_pResults);
	void OnTranslate(Translations * a_Callback);

//	void OnPerson(const ThingEvent & a_ThingEvent);
	void OnAgeTimeout();

};

#endif //SELF_TEXT_EXTRACTOR_H
