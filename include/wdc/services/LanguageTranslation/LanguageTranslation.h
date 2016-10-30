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

#ifndef WDC_LANGUAGETRANSLATION_H
#define WDC_LANGUAGETRANSLATION_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "services/IService.h"
#include "DataModels.h"
#include "WDCLib.h"		// include last always

class WDC_API LanguageTranslation : public IService
{
public:

    RTTI_DECL();

    //! Types
    typedef Delegate<Translations *>		OnTranslation;
    typedef Delegate<Languages *>			OnLanguage;
    typedef Delegate<IdentifiedLanguages *>	OnIdentifiedLanguages;

    //! Construction
    LanguageTranslation();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();
    virtual void GetServiceStatus( ServiceStatusCallback a_Callback );

    //! Translates input text from the source language to the target language.
    void Translation(const std::string & a_Source,
                     const std::string & a_Target,
                     const std::string & a_Text,
                     OnTranslation a_Callback);

    //! Return the list of languages it can detect.
    void IdentifiableLanguages(OnLanguage a_Callback);

    //! Identify the language in which a text is written.
    void Identify(const std::string & a_Text,
                  OnIdentifiedLanguages a_Callback);


private:

    //! This class is responsible for checking whether the service is available or not
    class ServiceStatusChecker {
        public:
            ServiceStatusChecker(LanguageTranslation *a_pLTService, ServiceStatusCallback a_Callback);

        private:
            LanguageTranslation *m_pLTService;
            IService::ServiceStatusCallback m_Callback;

            void OnCheckService(Languages *a_pLanguages);
    };
};


#endif //WDC_LANGUAGETRANSLATION_H
