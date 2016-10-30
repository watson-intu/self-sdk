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

#ifdef _WIN32

//! Disable this warning as it incorrectly warns over template classes like std::list<>
#pragma warning( disable:4251) // warning C4251: ... needs to have dll-interface to be used by clients of class ...

#undef WDC_API

#ifdef WDC_SERVICES_EXPORTS
#define WDC_API __declspec(dllexport)
#else
#define WDC_API __declspec(dllimport)
#endif

#else

// Linux, Android and other platforms don't need to export from a .SO
#define WDC_API

#endif
