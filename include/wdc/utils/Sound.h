/**
* Copyright 2016 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef WDC_SOUND_H
#define WDC_SOUND_H

#include "ISerializable.h"
#include "WDCLib.h"

class WDC_API Sound : public ISerializable
{
public:
	RTTI_DECL();

	// Construction
	Sound();
	Sound(const std::string & a_WaveData);

	// ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	// Accessors
	int	GetRate() const
	{
		return m_Rate;
	}

	int	GetChannels() const
	{
		return m_Channels;
	}

	int	GetBits() const
	{
		return m_Bits;
	}

	const std::string & GetWaveData() const
	{
		return m_WaveData;
	}

	float GetTime(int rate = -1) const
	{
		if (rate < 0)
			rate = m_Rate;

		int bytesPerSecond = rate * ((m_Bits >> 3) * m_Channels);
		return(float(m_WaveData.size()) / bytesPerSecond);
	}

	// Mutators
	void InitializeSound(int a_Rate, int a_Channels, int a_Bits, const std::string & a_WaveData);
	void Release();

	bool Load( const std::string & a_WaveData );
	bool Save( std::string & a_WaveData ) const;
	bool LoadFromFile(const std::string & a_FileName);
	bool SaveToFile(const std::string & a_FileName) const;

	// Static
	static bool	LoadWave(const std::string & a_FileData, int & a_Rate, int & a_Channels, int & a_Bits,
		std::string & a_WaveData);
	static bool SaveWave(std::string & a_FileData, int nRate, int nChannels, int nBits,
		const std::string & a_WaveData);

private:
	// Data
	int					m_Rate;				// sample rate
	int					m_Channels;			// number of channels
	int					m_Bits;				// bits per sample
	std::string			m_WaveData;
};


#endif

