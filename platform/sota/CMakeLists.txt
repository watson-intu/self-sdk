include_directories(.)

file(GLOB_RECURSE PLATFORM_SOTA_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
qi_create_lib(platform_sota SHARED ${PLATFORM_SOTA_CPP})
qi_use_lib(platform_sota self)
qi_stage_lib(platform_sota)


