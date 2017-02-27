  include_directories(. wiringPI)
  SET(GCC_COVERAGE_LINK_FLAGS "-lwiringPi")
  add_definitions(${GCC_COVERAGE_LINK_FLAGS} "-DBOOST_ASIO_DISABLE_STD_CHRONO -DBOOST_FILESYSTEM_VERSION=3")

  file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
  qi_create_lib(move_joint_plugin SHARED ${SELF_CPP})
  qi_use_lib(move_joint_plugin boost boost_system self wdc tinythread++)
  qi_stage_lib(move_joint_plugin)

  target_link_libraries(move_joint_plugin wiringPi)
