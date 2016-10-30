include_directories(".")

qi_create_lib(platform_ros SHARED
		RosPlatform.cpp
		gestures/RosMoveJointGesture.cpp
		tests/TestRosPlatform.cpp
	)
	
qi_use_lib(platform_ros self)
qi_stage_lib(platform_ros)

