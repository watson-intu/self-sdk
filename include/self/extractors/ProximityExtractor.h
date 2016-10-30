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

#ifndef SELF_PROXIMITY_EXTRACTOR_H
#define SELF_PROXIMITY_EXTRACTOR_H

#include <list>

#include "IFeatureExtractor.h"
#include "sensors/SensorManager.h"
#include "utils/Factory.h"
#include "SelfLib.h"



class SELF_API ProximityExtractor : public IFeatureExtractor
{
public:
	RTTI_DECL();

    ProximityExtractor() :
        m_MinSonarValue(0.0),
        m_MaxSonarValue(4.0),
        m_SamplesToAverage(5),
        m_CurrentAverage(0.0) 
    {}

    virtual void Deserialize(const Json::Value & json);
    virtual void Serialize(Json::Value & json);    

    //! IFeatureExtractor interface
    virtual const char * GetName() const;
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    typedef SensorManager::SensorList	SensorList;

    //! Data
    SensorList		        m_SonarSensors;
    SensorList              m_LaserSensors;
    float                   m_MinSonarValue;
    float                   m_MaxSonarValue;
    float                   m_CurrentAverage;
    int                     m_SamplesToAverage;

    //! Callback handler
    void OnSonarData(IData * data);
    void OnLaserData(IData * data);
};

#endif //SELF_PROXIMITY_EXTRACTOR_H
