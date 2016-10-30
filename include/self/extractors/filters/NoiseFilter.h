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

#ifndef SELF_NOISEFILTER_H
#define SELF_NOISEFILTER_H

#include <complex>
#include <valarray>
#include <cmath>
#include <fstream>

#include "FourierFilters.h"
#include "SelfLib.h"

class SELF_API NoiseFilter : public FourierFilters::IFourierFilter
{
public:
	RTTI_DECL();

	//! Types
	typedef std::complex<float>				ComplexNum;
	typedef std::valarray<ComplexNum> 		ComplexNumArray;
	typedef std::valarray<float>			NumArray;

	//! Construction
	NoiseFilter();

	//! ISerialziable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

    //! IFourierFilter interface
    virtual void ApplyFilter( ComplexNumArray & a_X );

private:
    //! Data
	NumArray							 	m_AvgFourierMagnitudes;
	int 									m_WindowSz;
	int										m_Count;
	bool									m_bPrimed;
	float									m_FiltCoeff;
};

#endif //SELF_NOISEFILTER_H
