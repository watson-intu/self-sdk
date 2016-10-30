cmake_minimum_required(VERSION 2.8.12.2)
project(self-sdk)
find_package(qibuild)
find_package(self)
find_package(wdc)

# Fix up the include paths for self & wdc
string(REPLACE "/include;" "/include/self;" _SELF_INCLUDES "${SELF_INCLUDE_DIRS}")
include_directories(${_SELF_INCLUDES})
string(REPLACE "/include;" "/include/wdc;" _WDC_INCLUDES "${WDC_INCLUDE_DIRS}")
include_directories(${_WDC_INCLUDES})

add_subdirectory(platform)
add_subdirectory(plugins)

