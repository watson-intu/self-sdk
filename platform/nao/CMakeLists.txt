add_definitions(" -DAUDIOIMPL_IS_REMOTE -DNAO_ENABLED ")
include_directories(../../platform/nao/)

file(GLOB_RECURSE NAO_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
qi_create_lib(platform_nao SHARED ${NAO_CPP})
qi_use_lib(platform_nao ALCOMMON ALPROXIES OPENCV2_CORE OPENCV2_HIGHGUI tinythread++ self qi)
qi_stage_lib(platform_nao)

