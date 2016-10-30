add_definitions(" -DBOOST_ASIO_DISABLE_STD_CHRONO -DBOOST_FILESYSTEM_VERSION=3")
include_directories(".")

qi_create_lib(sports_plugin SHARED
	services/Golf/Golf.cpp
	agent/proxies/GolfProxy.cpp
	services/Tennis/Tennis.cpp
	agent/proxies/TennisProxy.cpp
	)


if (APPLE)
	qi_use_lib(sports_plugin self wdc jsoncpp boost)
else()
	qi_use_lib(sports_plugin self wdc jsoncpp boost rt)
endif()

qi_stage_lib(sports_plugin)


