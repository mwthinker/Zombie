include(FetchContent)
# Load external github projects
FetchContent_Declare(CMakeHelper
	GIT_REPOSITORY
		https://github.com/mwthinker/CMakeHelper.git
	GIT_TAG
		a321588de57895a85e642810b6eedc5cbf89c0ea
)
FetchContent_MakeAvailable(CMakeHelper)
FetchContent_GetProperties(CMakeHelper
	SOURCE_DIR
		CMakeHelper_SOURCE_DIR
)
include(${CMakeHelper_SOURCE_DIR}/auxiliary.cmake)

message(STATUS "Download private data repository is available to add: -DZombieData_PrivateRepo_SSH=1")
message(STATUS "Download private data repository is available to add: -DZombieData_PrivateRepo_HTTPS=1")
option(ZombieData_PrivateRepo_SSH "Add ZombieData_PrivateRepo_SSH to project." OFF)
option(ZombieData_PrivateRepo_HTTPS "Add ZombieData_PrivateRepo_HTTPS to project." OFF)

if (ZombieData_PrivateRepo_SSH OR ZombieData_PrivateRepo_HTTPS)
	set(ZombieData ON)
	set(ZombieData_PrivateRepo_URL "https://github.com/mwthinker/ZombieData.git")
	if (ZombieData_PrivateRepo_SSH)
		set(ZombieData_PrivateRepo_URL "git@github.com:mwthinker/ZombieData.git")
	endif ()

	FetchContent_Declare(ZombieData
		GIT_REPOSITORY
			${ZombieData_PrivateRepo_URL}
		GIT_TAG
			28fa5d69bb07b8963f9c7d2d65434350bd36d1db
	)
	FetchContent_MakeAvailable(ZombieData)
	FetchContent_GetProperties(ZombieData
		SOURCE_DIR
			ZombieData_SOURCE_DIR
	)
endif ()
