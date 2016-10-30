include_directories(".")
add_definitions( "-DBOOST_ASIO_DISABLE_STD_CHRONO -DBOOST_FILESYSTEM_VERSION=3")

qi_create_lib(boost_1_60_0 STATIC
		libs/thread/src/pthread/once.cpp
		libs/thread/src/pthread/once_atomic.cpp
		libs/thread/src/pthread/thread.cpp
		libs/system/src/error_code.cpp
)

qi_stage_lib(boost_1_60_0)


