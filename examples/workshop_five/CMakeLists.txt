  include_directories(. wiringPI)
  SET(GCC_COVERAGE_LINK_FLAGS "-lwiringPi")
  add_definitions(${GCC_COVERAGE_LINK_FLAGS})

  file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
  qi_create_lib(workshop_five_plugin SHARED ${SELF_CPP})
  qi_use_lib(workshop_five_plugin self wdc)
  qi_stage_lib(workshop_five_plugin)

  target_link_libraries(workshop_five_plugin wiringPi)
