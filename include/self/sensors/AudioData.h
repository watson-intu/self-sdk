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

#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include "IData.h"
#include "sensors/ISensor.h"
#include "utils/Sound.h"
#include "SelfLib.h"

//! This data type contains raw wave audio data..
class SELF_API AudioData : public IData
{
public:
	RTTI_DECL();

	//! Construction
	AudioData() : m_Freq( 0 ), m_Channels( 0 ), m_BPS( 0 )
	{}
	AudioData(const std::string & a_WaveData, unsigned int a_Freq, unsigned int a_Channels, unsigned int a_BPS ) :
		m_WaveData( a_WaveData ), m_Freq( a_Freq ), m_Channels( a_Channels ), m_BPS( a_BPS )
	{}

	//! ISerializable interface
	virtual void Serialize(Json::Value & json)
	{
		json["m_WaveData"] = StringUtil::EncodeBase64( m_WaveData );
		json["m_Freq"] = m_Freq;
		json["m_Channels"] = m_Channels;
		json["m_BPS"] = m_BPS;
	}

	virtual void Deserialize(const Json::Value & json)
	{
		m_WaveData = StringUtil::DecodeBase64( json["m_WaveData"].asString() );
		m_Freq = json["m_Freq"].asUInt();
		m_Channels = json["m_Channels"].asUInt();
		m_BPS = json["m_BPS"].asUInt();
	}

	//! IData interface
	virtual bool ToBinary( std::string & a_Output )
	{
		a_Output = m_WaveData;
		return true;
	}
	virtual bool FromBinary( const std::string & a_Type, const std::string & a_Input )
	{
		std::vector<std::string> parts;
		StringUtil::Split( a_Type, ";", parts );

		if ( parts.size() > 0 && StringUtil::Compare( parts[0], "audio/L16", true ) == 0 )
		{
			m_BPS = 16;
			m_Freq = 16000;
			m_Channels = 1;

			for(size_t i=1;i<parts.size();++i)
			{
				std::vector<std::string> kv;
				StringUtil::Split( parts[i], "=", kv );
				if ( kv.size() == 2 )
				{
					if ( StringUtil::Compare( kv[0], "rate", true ) == 0 )
						m_Freq = atoi( kv[1].c_str() );
					else if ( StringUtil::Compare( kv[0], "channels", true ) == 0 )
						m_Channels = atoi( kv[1].c_str() );
				}
			}

			m_WaveData = a_Input;
			return true;
		}

		return false;
	}

	const std::string & GetWaveData() const
	{
		return m_WaveData;
	}

	unsigned int GetFrequency() const
	{
		return m_Freq;
	}
	unsigned int GetChannels() const
	{
		return m_Channels;
	}
	unsigned int GetBPS() const
	{
		return m_BPS;
	}

private:
	//! Data
	std::string			m_WaveData;
	unsigned int		m_Freq;
	unsigned int		m_Channels;
	unsigned int		m_BPS;		// bits per sample
};

#endif
