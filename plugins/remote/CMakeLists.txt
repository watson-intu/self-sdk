add_definitions(" -DAUDIOIMPL_IS_REMOTE -DNAO_ENABLED -DBOOST_ASIO_DISABLE_STD_CHRONO -DBOOST_FILESYSTEM_VERSION=3")
include_directories(. ../../lib)

file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
qi_create_lib(remote_plugin SHARED ${SELF_CPP})
qi_use_lib(remote_plugin self wdc tinythread++)
qi_stage_lib(remote_plugin)

