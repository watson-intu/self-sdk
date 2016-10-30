if("$ENV{TARGET}" STREQUAL "")
	message(warning " ---- No Target defined, defaulting to Nao.")
	set(ENV{TARGET} "NAO")
endif()

if("$ENV{TARGET}" STREQUAL "NAO")
	add_subdirectory(nao)
endif()

if("$ENV{TARGET}" STREQUAL "LINUX")
	add_subdirectory(linux)
endif()

if("$ENV{TARGET}" STREQUAL "SOTA")
	add_subdirectory(sota)
endif()

if("$ENV{TARGET}" STREQUAL "MAC")
	add_subdirectory(mac)
endif()

