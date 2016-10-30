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

#ifndef SELF_IMAGEEXTRACTOR_H
#define SELF_IMAGEEXTRACTOR_H

#include <list>

#include "IFeatureExtractor.h"
#include "sensors/SensorManager.h"
#include "utils/Factory.h"
#include "SelfLib.h"


class SELF_API ImageExtractor : public IFeatureExtractor
{
public:
	RTTI_DECL();

    ImageExtractor() {}


    //! IFeatureExtractor interface
    virtual const char * GetName() const;
    virtual bool OnStart();
    virtual bool OnStop();

private:
    //! Types
    typedef SensorManager::SensorList	SensorList;

    //! Data
    SensorList		        m_VideoSensors;

    //! Callback handler
    void OnVideoData(IData * data);
};

#endif //SELF_IMAGEEXTRACTOR_H
