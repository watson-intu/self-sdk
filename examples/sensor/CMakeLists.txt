add_definitions(" -DAUDIOIMPL_IS_REMOTE -DNAO_ENABLED -DBOOST_ASIO_DISABLE_STD_CHRONO -DBOOST_FILESYSTEM_VERSION=3")
include_directories(.)

file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
qi_create_lib(sensor_plugin SHARED ${SELF_CPP})
qi_use_lib(sensor_plugin self wdc tinythread++ boost boost_system)
qi_stage_lib(sensor_plugin)

