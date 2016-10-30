/**
* Copyright 2015 IBM Corp. All Rights Reserved.
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

#include "MergersAndAcquisitions.h"
#include "SelfInstance.h"

REG_SERIALIZABLE( MergersAndAcquisitions );
RTTI_IMPL( MergersAndAcquisitions, IService );


MergersAndAcquisitions::MergersAndAcquisitions() : IService("MergersAndAcquisitionsV1")
{}

//! ISerializable
void MergersAndAcquisitions::Serialize(Json::Value & json)
{
    IService::Serialize(json);
}

void MergersAndAcquisitions::Deserialize(const Json::Value & json)
{
    IService::Deserialize(json);
}

//! IService interface
bool MergersAndAcquisitions::Start()
{
    if (!IService::Start())
        return false;

    Log::Debug("MergersAndAcquisitions", "Url loaded as %s", m_pConfig->m_URL.c_str() );
    return true;
}

void MergersAndAcquisitions::SendRequest(const std::string & a_Input,
    Delegate<const std::string &> a_Callback, 
	int a_nMaxRetreive /*= 5*/,
	bool a_bUseCache /*= true*/ )
{
    Json::Value revenueGrowth;
    revenueGrowth["halfFactor"] = 3;
    revenueGrowth["max"] = 222;
    revenueGrowth["min"] = 2;
    revenueGrowth["scoreThreshold"] = 0;

	Json::Value revenue;
	revenue["halfFactor"] = 5;
    revenue["max"] = 9999999;
    revenue["min"] = 33;
    revenue["scoreThreshold"] = 0;

	Json::Value location;
	location["content"] = "";

	Json::Value employees;
	employees["halfFactor"] = 3;
    employees["max"] = 100;
    employees["min"] = 10;
    employees["scoreThreshold"] = 0;

	Json::Value businessDescription;
	businessDescription["conceptThreshold"] = 0;
    businessDescription["content"] = a_Input;
    businessDescription["maxRetrieve"] = 300;
    businessDescription["preferenceWeights"] = 0;
    businessDescription["scoreThreshold"] = 0.2;
    businessDescription["scoringMethod"] = "words";

	Json::Value subcriteria;
	subcriteria["businessDescription"] = businessDescription;
    subcriteria["employees"] = employees;
    subcriteria["location"] = location;
    subcriteria["revenue"] = revenue;
    subcriteria["revenueGrowth"] = revenueGrowth;

	Json::Value criteria;
	criteria["subcriteria"] = subcriteria;
	criteria["maxRetreive"] = a_nMaxRetreive;

	Json::Value req;
	req["criteria"] = criteria;

	Headers headers;
	headers["Content-Type"] = "application/json";

    Log::Debug("MergersAndAcquisitions", "Sending the following payload: %s", req.toStyledString().c_str());
    new RequestData(this, EMPTY_STRING, "POST", headers, req.toStyledString(), a_Callback,
		a_bUseCache ? new CacheRequest( "SendRequest", JsonHelpers::Hash( req ) ) : NULL );
}
