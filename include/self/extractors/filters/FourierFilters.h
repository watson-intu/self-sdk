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

#ifndef SELF_FOURIERFILTERS_H
#define SELF_FOURIERFILTERS_H

#include <complex>
#include <valarray>
#include <cmath>
#include <fstream>

#include "extractors/TextExtractor.h"
#include "SelfLib.h"

class SELF_API FourierFilters : public TextExtractor::IAudioFilter
{
public:
	RTTI_DECL();

	//! Types
	typedef std::complex<float>					ComplexNum;
	typedef std::valarray<ComplexNum> 			ComplexNumArray;

	//! Interface class for any audio filter that operates in the frequency domain
	class SELF_API IFourierFilter : public ISerializable,
		public boost::enable_shared_from_this<IFourierFilter>
	{
	public:
		RTTI_DECL();

		//! Types
		typedef boost::shared_ptr<IFourierFilter>		SP;
		typedef boost::weak_ptr<IFourierFilter>			WP;
		//! Interface
		virtual void ApplyFilter( ComplexNumArray & a_Data ) = 0;
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
	FourierFilters();

	//! ISerialziable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

    //! IAudioFilter interface
    virtual void DoApplyFilter( SpeechAudioData & a_Data );

	//! Mutators
	void AddFilter( IFourierFilter::SP a_Filter );

private:
    typedef std::vector<IFourierFilter::SP>		Filters;

    //! Data
	Filters									m_Filters;
	int										m_LastInputSz;
	int										m_LastBitRate;
	int										m_NextPow2;

	//! Helpers for Background Noise Filtering
	void FFT(ComplexNumArray & raw_audio);
	void IFFT(ComplexNumArray & filtered_frequency_components);
	int NextPow2(int i);

	//! Debugging

    void SaveArrayToCSV(const ComplexNumArray & array, const char * filename);
};

#endif //SELF_FOURIERFILTERS_H
