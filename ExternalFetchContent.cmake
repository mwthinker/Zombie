include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
	GIT_REPOSITORY
		https://github.com/mwthinker/CppSdl2.git
	GIT_TAG
		0501a37124649da9c6a2a0f0ebed563132a44d6f
)
FetchContent_MakeAvailable(CppSdl2)

FetchContent_Declare(Signal
	GIT_REPOSITORY
		https://github.com/mwthinker/Signal.git
	GIT_TAG
		c05de2bf17961b7b094582802eff9107becfa08a
)
FetchContent_MakeAvailable(Signal)

fetchcontent_declare(CmakeAuxiliary
	GIT_REPOSITORY
		https://gist.github.com/mwthinker/e0539b200e3f0ac6da545af4843a6ff6
)
FetchContent_MakeAvailable(CmakeAuxiliary)
FetchContent_GetProperties(CmakeAuxiliary
	SOURCE_DIR
		CmakeAuxiliary_SOURCE_DIR
)
include(${CmakeAuxiliary_SOURCE_DIR}/auxiliary.cmake)

message(STATUS "Download private date repository is available to add: -DZombieData_PrivateRepo=1")
option(ZombieData_PrivateRepo "Add ZombieData_PrivateRepo to project." OFF)
if (ZombieData_PrivateRepo)
	# Load data.
	FetchContent_Declare(ZombieData
		GIT_REPOSITORY
			https://github.com/mwthinker/ZombieData.git
		GIT_TAG
			28fa5d69bb07b8963f9c7d2d65434350bd36d1db
	)
	FetchContent_MakeAvailable(ZombieData)
	FetchContent_GetProperties(ZombieData
		SOURCE_DIR
			ZombieData_SOURCE_DIR
	)
endif()
