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

#ifndef SELF_GESTUREEXTRACTOR_H
#define SELF_GESTUREEXTRACTOR_H

#include <list>

#include "IFeatureExtractor.h"
#include "sensors/SensorManager.h"
#include "utils/Factory.h"
#include "SelfLib.h"


class SELF_API GestureExtractor : public IFeatureExtractor
{
public:
RTTI_DECL();

    GestureExtractor() {}


    //! IFeatureExtractor interface
    virtual const char * GetName() const;
    virtual bool OnStart();
    virtual bool OnStop();

    private:
    //! Types
    typedef SensorManager::SensorList	SensorList;

    //! Data
    SensorList		        m_GazeSensors;
    SensorList              m_GestureSensors;

    //! Callback handler
    void OnGazeData(IData * data);
    void OnGestureData(IData * data);

};

#endif //SELF_GESTUREEXTRACTOR_H
