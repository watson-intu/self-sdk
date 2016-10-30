add_definitions(" -DBOOST_ASIO_DISABLE_STD_CHRONO -DBOOST_FILESYSTEM_VERSION=3")
include_directories(".")

qi_create_lib(face_plugin SHARED
		agent/FaceAgent.cpp
		blackboard/NameIntent.cpp
		classifiers/FaceClassifier.cpp
		services/FacialRecognition.cpp
		tests/TestFacialRecognition.cpp
	)
qi_use_lib(face_plugin jsoncpp boost boost_filesystem boost_thread boost_system self )
qi_stage_lib(face_plugin)


