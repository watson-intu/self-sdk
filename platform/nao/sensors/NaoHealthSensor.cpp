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

#include "NaoHealthSensor.h"
#include "NaoPlatform.h"
#include "utils/AlHelpers.h"

#ifndef _WIN32
#include <alproxies/almemoryproxy.h>
#include <alproxies/alsensorsproxy.h>
#include <alproxies/alsystemproxy.h>
#include <alproxies/albatteryproxy.h>
#include <alproxies/alaudiodeviceproxy.h>
#include <alvalue/alvalue.h>
#endif

REG_SERIALIZABLE(NaoHealthSensor);
REG_OVERRIDE_SERIALIZABLE(HealthSensor, NaoHealthSensor);
RTTI_IMPL(NaoHealthSensor, HealthSensor);


void NaoHealthSensor::Serialize(Json::Value & json)
{
	HealthSensor::Serialize(json);
	json["m_RepairThreshold"] = m_RepairThreshold;
	json["m_bEnableFallRecovery"] = m_bEnableFallRecovery;
	json["m_fTimeForPostureTransition"] = m_fTimeForPostureTransition;
	json["m_fLowTempThreshold"] = m_fLowTempThreshold;
	json["m_fHighTempThreshold"] = m_fHighTempThreshold;
	json["m_fHeadHighTempThreshold"] = m_fHeadHighTempThreshold;
	json["m_CriticalFailureLifespan"] = m_CriticalFailureLifespan;
	json["m_CriticalFailureBufferTime"] = m_CriticalFailureBufferTime;
	json["m_RepairAlertBufferTime"] = m_RepairAlertBufferTime;
	json["m_LowBatteryBufferTime"] = m_LowBatteryBufferTime;
	json["m_AbnormalTempBufferTime"] = m_AbnormalTempBufferTime;
	SerializeVector("m_StablePostures", m_StablePostures, json);
	SerializeVector("m_ImportantJoints", m_ImportantJoints, json);
}

void NaoHealthSensor::Deserialize(const Json::Value & json)
{
	HealthSensor::Deserialize(json);
	if (json.isMember("m_RepairThreshold"))
		m_RepairThreshold = json["m_RepairThreshold"].asInt();
	if (json.isMember("m_bEnableFallRecovery"))
		m_bEnableFallRecovery = json["m_bEnableFallRecovery"].asBool();
	if (json.isMember("m_fTimeForPostureTransition"))
		m_fTimeForPostureTransition = json["m_fTimeForPostureTransition"].asFloat();
	if (json.isMember("m_fLowTempThreshold"))
		m_fLowTempThreshold = json["m_fLowTempThreshold"].asFloat();
	if (json.isMember("m_fHighTempThreshold"))
		m_fHighTempThreshold = json["m_fHighTempThreshold"].asFloat();
	if (json.isMember("m_fHeadHighTempThreshold"))
		m_fHeadHighTempThreshold = json["m_fHeadHighTempThreshold"].asFloat();
	if (json.isMember("m_CriticalFailureLifespan"))
		m_CriticalFailureLifespan = json["m_CriticalFailureLifespan"].asDouble();
	if (json.isMember("m_CriticalFailureBufferTime"))
		m_CriticalFailureBufferTime = json["m_CriticalFailureBufferTime"].asDouble();
	if (json.isMember("m_RepairAlertBufferTime"))
		m_RepairAlertBufferTime = json["m_RepairAlertBufferTime"].asDouble();
	if (json.isMember("m_LowBatteryBufferTime"))
		m_LowBatteryBufferTime = json["m_LowBatteryBufferTime"].asDouble();
	if (json.isMember("m_AbnormalTempBufferTime"))
		m_AbnormalTempBufferTime = json["m_AbnormalTempBufferTime"].asDouble();
	DeserializeVector("m_StablePostures", json, m_StablePostures);
	if ( m_StablePostures.size() == 0 )
		m_StablePostures.push_back( "Stand" );
	DeserializeVector("m_ImportantJoints", json, m_ImportantJoints);
	if ( m_ImportantJoints.size() == 0 )
		m_ImportantJoints.push_back( "*Head*Temperature*" );
}

bool NaoHealthSensor::OnStart()
{
	Log::Debug("NaoHealthSensor", "OnStart() invoked.");

	m_StopThread = false;
	m_ThreadStopped = false;

	if (m_SensorReadings.size() == 0)
		m_SensorReadings.push_back("Device/SubDeviceList/LHand/Temperature/Sensor/Value");
	if (m_ErrorDiagnosis.size() == 0)
		m_ErrorDiagnosis.push_back("Diagnosis/Active/HeadYaw/Error");

	pPlatform = NaoPlatform::Instance();
	m_PostureNotifier = DELEGATE(NaoHealthSensor, OnPostureChanged, const std::string &, this);
	pPlatform->AddPostureNotifier(m_PostureNotifier);

	NaoPlatform * pPlatform = NaoPlatform::Instance();
	if (pPlatform->HasService("ALMemory"))
	{
		m_Memory = pPlatform->GetSession()->service("ALMemory");
		m_BatteryPowerSub = m_Memory.call<qi::AnyObject>("subscriber", "BatteryChargingFlagChanged");
		m_BatteryPowerSub.connect("signal", qi::AnyFunction::fromDynamicFunction(boost::bind(&NaoHealthSensor::OnBatteryPowerChanged, this, _1)));
	}

	// create thread to get event notification
	ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(NaoHealthSensor, SensorDiagnosis, void *, this), NULL);
	return true;
}

bool NaoHealthSensor::OnStop()
{
	Log::Debug("NaoHealthSensor", "OnStop() invoked.");
	m_StopThread = true;
	m_Memory.reset();
	pPlatform->RemovePostureNotifier(this);
	while (!m_ThreadStopped)
		tthread::this_thread::yield();
	return true;
}

void NaoHealthSensor::SensorDiagnosis(void * arg)
{
	try
	{
		CallSensorDiagnosis(arg);
	}
	catch (const std::exception & ex)
	{
		Log::Error("NaoHealthSensor", "Caught Exception: %s", ex.what());
	}
	m_ThreadStopped = true;
}

void NaoHealthSensor::OnPostureChanged( const std::string& posture )
{
	m_LastPosture = posture;

	BuildHealthData("PostureChanged", "", m_LastPosture.c_str(), 0.0f, false, false);

	if ( IsStablePosture(m_LastPosture) ) 	// if current posture is stable, reset fallen count
		m_RobotFallenCount = 0;

	m_LastPostureTime = Time();
}

qi::AnyReference NaoHealthSensor::OnBatteryPowerChanged( const std::vector<qi::AnyReference> & args )
{
	for (size_t i = 0; i < args.size(); ++i)
	{
		if (args[i].content().kind() == qi::TypeKind_Int)
		{
			m_bBatteryCharging = args[i].content().toInt() ? true : false;
			Log::DebugLow("Health", "OnBatteryPowerChanged: %d", m_bBatteryCharging);

			BuildHealthData("batteryCharging", "", m_bBatteryCharging ? "plugged" : "unplugged", 0.0f, false, false);
		}
	}
	return qi::AnyReference();
}

void NaoHealthSensor::CallSensorDiagnosis(void * arg)
{
#ifndef _WIN32
	std::string robotIp("127.0.0.1");
	if ( SelfInstance::GetInstance() != NULL )
		robotIp = SelfInstance::GetInstance()->GetRobotIp();
	AL::ALMemoryProxy mem(robotIp.c_str());

	AL::ALSensorsProxy sensorProxy(robotIp.c_str());
	for (size_t i = 0; i < m_SensorReadings.size(); ++i)
		sensorProxy.subscribe(m_SensorReadings[i]);
	for (size_t i = 0; i < m_ErrorDiagnosis.size(); ++i)
		sensorProxy.subscribe(m_ErrorDiagnosis[i]);

	AL::ALSystemProxy systemProxy(robotIp.c_str());
	AL::ALBatteryProxy batteryProxy(robotIp.c_str());
	AL::ALAudioDeviceProxy audioDeviceProxy(robotIp.c_str());

	Time lastCheck;
	while (!m_StopThread)
	{
		if ((Time().GetEpochTime() - lastCheck.GetEpochTime()) > m_HealthSensorCheckInterval)
		{
			lastCheck = Time();

			Log::Debug("NaoHealthSensor", "m_RobotFallenCount: %d", m_RobotFallenCount.load());
			// robot fall recovery
			if ( m_bEnableFallRecovery )
			{
				if ( !m_LastPosture.empty() && !IsStablePosture(m_LastPosture) )
				{
					if ( m_RobotFallenCount > 0 )
					{
						Log::Status("Health", "Failed to recover from posture: %s", m_LastPosture.c_str());

						BuildHealthData("FallenRobotRecoveryFailure", "", "CRITICAL", (float) m_RobotFallenCount, true, true);
					}
					// if last posture time was < ~12 seconds ago, the robot is in the middle of a transition so should not do anything
					else if ( Time().GetEpochTime() - m_LastPostureTime.GetEpochTime() > m_fTimeForPostureTransition )
					{
						Log::Status("Health", "Trying to recover from fall from posture: %s", m_LastPosture.c_str());
						qi::Future<bool> action = pPlatform->SetPosture("Stand", 0.8);
						action.wait();
						m_RobotFallenCount++;
					}
				}
				else
				{
					m_RobotFallenCount = 0;
				}
			}

			// System values
			BuildHealthData("systemVersion", "", systemProxy.systemVersion().c_str(), 0.0f, false, false);

			BuildHealthData("timezone", "", systemProxy.timezone().c_str(), 0.0f, false, false);

			float batteryCharge = (float) batteryProxy.getBatteryCharge();
			if ( batteryCharge < m_fLowBatteryThreshold && !m_bBatteryCharging )
			{
				Log::Error("Health", "Low Battery Charge at %f", batteryCharge);
				if ( (Time().GetEpochTime() - m_LastLowBattery) > m_LowBatteryBufferTime )
				{
					BuildHealthData("BatteryLow", "", "CRITICAL", batteryCharge, true, true);
					m_LastLowBattery = Time().GetEpochTime();
				}
				else
					BuildHealthData("BatteryLow", "", "CRITICAL", batteryCharge, true, false);
			}
			else
				BuildHealthData("batteryCharge", "", m_bBatteryCharging ? "plugged" : "unplugged", batteryCharge, false, false);

			// volume
			BuildHealthData("volume", "", "", (float) audioDeviceProxy.getOutputVolume(), false, false);
			BuildHealthData("audioOut", "", audioDeviceProxy.isAudioOutMuted() ? "Muted" : "NotMuted", 0.0f, false, false);

			// Device values
			for (size_t i = 0; i < m_SensorReadings.size(); ++i)
			{
				// check that the key is a valid one
				std::vector<std::string> matchingKeys = mem.getDataList(m_SensorReadings[i]);
				if (matchingKeys.size() > 0)
				{
					AL::ALValue memData = mem.getData(m_SensorReadings[i]);

					std::string simplifiedKey = SimplifyKey(m_SensorReadings[i]);
					Log::DebugLow("Health", "%s - %s", simplifiedKey.c_str(), AlHelpers::DumpValue(memData).c_str());

					if (memData.isFloat() || memData.isInt())
					{
						if ( (IsHeadJoint(simplifiedKey) && (float) memData > m_fHeadHighTempThreshold) ||
						     (IsImportantJoint(simplifiedKey) && ( (float) memData > m_fHighTempThreshold || (float) memData < m_fLowTempThreshold )) )
						{
							if ( (Time().GetEpochTime() - m_LastAbnormalTemp) > m_AbnormalTempBufferTime )
							{
								BuildHealthData("TemperatureAbnormal", simplifiedKey.c_str(), "CRITICAL", (float) memData, true, !IsRestPosture());
								m_LastAbnormalTemp = Time().GetEpochTime();
							}
							else
								BuildHealthData("TemperatureAbnormal", simplifiedKey.c_str(), "CRITICAL", (float) memData, true, false);
						}
						else
						{
							BuildHealthData(simplifiedKey.c_str(), simplifiedKey.c_str(), "", (float) memData, false, false);
						}
					}
					else
					{
						BuildHealthData(simplifiedKey.c_str(), simplifiedKey.c_str(), AlHelpers::ToString(memData).c_str(), 0.0f, false, false);
					}
				}
				else
					Log::Debug("NaoHealthSensor", "%s not found", m_SensorReadings[i].c_str());
			}

			// Error diagnosis
			for (size_t i = 0; i < m_ErrorDiagnosis.size(); ++i)
			{
				// check that the key is a valid one
				std::vector<std::string> matchingKeys = mem.getDataList(m_ErrorDiagnosis[i]);
				if (matchingKeys.size() > 0)
				{
					AL::ALValue memData = mem.getData(m_ErrorDiagnosis[i]);

					std::string simplifiedKey = SimplifyKey(m_ErrorDiagnosis[i]);

					if (memData.isInt())
					{
						int errorCode = (int)memData; // level of failure severity: 0 (NEGLIGIBLE), 1 (SERIOUS) or 2 (CRITICAL).

						std::string healthState = "";
						if (errorCode == 0)
							healthState = "NEGLIGIBLE";
						else if (errorCode == 1)
						{
							healthState = "SERIOUS";
							Log::DebugLow("Health", "%s: %s", simplifiedKey.c_str(), AlHelpers::DumpValue(memData).c_str());
						}
						else if (errorCode == 2)
						{
							healthState = "CRITICAL";
							Log::Error("Health", "%s: %s", simplifiedKey.c_str(), AlHelpers::DumpValue(memData).c_str());
							std::map<std::string, double>::iterator it = m_FailedSensors.find(simplifiedKey);
							if (it == m_FailedSensors.end()) {
								m_FailedSensors[simplifiedKey] = Time().GetEpochTime();
							}
						}

						if ( healthState == "CRITICAL" )
						{
							if ( (Time().GetEpochTime() - m_LastCriticalFailure) > m_CriticalFailureBufferTime )
							{
								BuildHealthData("SensorErrorCritical", simplifiedKey.c_str(), healthState.c_str(),
								                (float) errorCode, (errorCode > 0) ? true : false, !IsRestPosture());
								m_LastCriticalFailure = Time().GetEpochTime();
							}
							else
							{
								BuildHealthData("SensorErrorCritical", simplifiedKey.c_str(), healthState.c_str(),
								                (float) errorCode, (errorCode > 0) ? true : false, false);
							}
						}
						else
						{
							std::map<std::string, double>::iterator it2 = m_FailedSensors.find(simplifiedKey);
							if ( it2 != m_FailedSensors.end() )
							{
								m_FailedSensors.erase(it2);

								double failureTime = it2->second;
								m_RecoveredSensors[StringUtil::Format("%s: %f", simplifiedKey.c_str(), failureTime)] = failureTime;
								Log::DebugHigh("HealthAgent", "Sensor has recovered so adding to recoveredSensorMap key: %s value: %f",
								               StringUtil::Format("%s: %f", simplifiedKey.c_str(), failureTime).c_str(), failureTime);
							}
							BuildHealthData("SensorErrorCritical", simplifiedKey.c_str(), healthState.c_str(),
							                (float) errorCode, (errorCode > 0) ? true : false, !IsRestPosture());
						}
					}
					else
					{
						Log::DebugLow("Health", "%s: %s", simplifiedKey.c_str(), AlHelpers::DumpValue(memData).c_str());
					}
				}
				else
					Log::DebugLow("Health", "%s not found", m_ErrorDiagnosis[i].c_str());
			}

			// if a recovery occurred a while ago, remove it from the map
			FilterRecoveredSensorsMap();

			// if number of critical errors exceed threshold, create a repair goal
			int failuresInWindow = m_FailedSensors.size() + m_RecoveredSensors.size();
			if ( failuresInWindow >= m_RepairThreshold )
			{
				if ( (Time().GetEpochTime() - m_LastRepairAlert) > m_RepairAlertBufferTime )
				{
					Log::Error("Health", "Failed sensors count: %d so creating a SensorErrorRepair goal", m_FailedSensors.size());
					BuildHealthData("SensorErrorRepair", "", "CRITICAL", (float) failuresInWindow, true, !IsRestPosture());
					m_LastRepairAlert = Time().GetEpochTime();
				}
			}
		}
		else
		{
			tthread::this_thread::sleep_for(tthread::chrono::milliseconds(50));
		}
	}

	for (size_t i = 0; i < m_SensorReadings.size(); ++i)
		sensorProxy.unsubscribe(m_SensorReadings[i]);
	for (size_t i = 0; i < m_ErrorDiagnosis.size(); ++i)
		sensorProxy.unsubscribe(m_ErrorDiagnosis[i]);

#endif
}

bool NaoHealthSensor::IsStablePosture(const std::string & posture)
{
	for(size_t i=0; i<m_StablePostures.size();++i)
	{
		std::string prefix(m_StablePostures[i]);
		if (!posture.compare(0, prefix.size(), prefix))
		{
			return true;
		}
	}
	return false;
}

bool NaoHealthSensor::IsHeadJoint(const std::string & joint)
{
	if ( StringUtil::WildMatch( "Head*Temperature*", joint ) )
		return true;

	return false;
}

bool NaoHealthSensor::IsImportantJoint(const std::string & joint)
{
	for(size_t i=0;i<m_ImportantJoints.size();++i)
		if ( StringUtil::WildMatch( m_ImportantJoints[i].c_str(), joint ) )
			return true;

	return false;
}

std::string NaoHealthSensor::SimplifyKey(const std::string & key)
{
	std::string simpleKey(key);
	size_t diagnosisPrefix = StringUtil::Find(key, "Diagnosis/Active/");

	if (diagnosisPrefix != -1)
		simpleKey = key.substr(diagnosisPrefix + 17);
	else
	{
		size_t valuePrefix = StringUtil::Find(key, "Device/SubDeviceList/");
		size_t valueSuffix = StringUtil::Find(key, "Sensor/Value");

		if (valuePrefix == 0 && valueSuffix != -1)
			simpleKey = key.substr(valuePrefix + 21, valueSuffix - valuePrefix - 22);
	}

	if ( StringUtil::StartsWith(simpleKey, "L") )
		simpleKey = "Left" + simpleKey.erase(0,1);

	if ( StringUtil::StartsWith(simpleKey, "R") )
		simpleKey = "Right" + simpleKey.erase(0,1);

	return simpleKey;
}

void NaoHealthSensor::BuildHealthData( const char * name, const char * sensor, const char * state,
                                       float value, bool error, bool raiseAlert )
{
	Json::Value paramsMap;
	if ( strlen(sensor) > 0 )
		paramsMap["sensor"] = sensor;
	paramsMap["state"] = state;
	paramsMap["value"] = value;
	paramsMap["error"] = error;
	paramsMap["raiseAlert"] = raiseAlert;
	ThreadPool::Instance()->InvokeOnMain(DELEGATE(NaoHealthSensor, SendHealthData, HealthData * , this),
	                                     new HealthData(name, paramsMap));
}

void NaoHealthSensor::SendHealthData(HealthData * a_pData)
{
	SendData(a_pData);
}

bool NaoHealthSensor::IsRestPosture()
{
	return m_LastPosture == "Crouch";
}

void NaoHealthSensor::FilterRecoveredSensorsMap()
{
	for (std::map<std::string, double>::iterator it = m_RecoveredSensors.begin(); it != m_RecoveredSensors.end(); ++it)
	{
		if ( ( Time().GetEpochTime() - it->second) > m_CriticalFailureLifespan )
		{
			Log::DebugHigh("Health", "Sensor recovery %s exceeds life span so removing from recovered map size: %d",
			               it->first.c_str(), m_RecoveredSensors.size());
			m_RecoveredSensors.erase(it);
		}
	}
}