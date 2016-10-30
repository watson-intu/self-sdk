set(CMAKE_MACOSX_RPATH 1)
include_directories(".")

qi_create_lib(platform_mac SHARED
        gestures/AQSpeechGesture.cpp
        sensors/AQMicrophone.cpp)

qi_use_lib(platform_mac self wdc qi portaudio)

qi_stage_lib(platform_mac)
