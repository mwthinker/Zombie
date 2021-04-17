include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
	GIT_REPOSITORY
		https://github.com/mwthinker/CppSdl2.git
	GIT_TAG
		e47fdfd75cbd7e9f8baa2ddb8bb17aa593fb3151
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

message(STATUS "Download private date repository is available to add: -DZombieData_PrivateRepo_SSH=1")
message(STATUS "Download private date repository is available to add: -DZombieData_PrivateRepo_HTTPS=1")
option(ZombieData_PrivateRepo_SSH "Add ZombieData_PrivateRepo_SSH to project." OFF)
option(ZombieData_PrivateRepo_HTTPS "Add ZombieData_PrivateRepo_HTTPS to project." OFF)

if (ZombieData_PrivateRepo_SSH OR ZombieData_PrivateRepo_HTTPS)
	set(ZombieData_PrivateRepo OFF)
	set(ZombieData_PrivateRepo_HTTPS "https://github.com/mwthinker/ZombieData.git")
	if (ZombieData_PrivateRepo_SSH)
		set(ZombieData_PrivateRepo_SSH "git@github.com:mwthinker/Zombie.git")
	endif ()
	# Load data.
	FetchContent_Declare(ZombieData
		GIT_REPOSITORY
			${ZombieData_PrivateRepo_SSH}
		GIT_TAG
			28fa5d69bb07b8963f9c7d2d65434350bd36d1db
	)
	FetchContent_MakeAvailable(ZombieData)
	FetchContent_GetProperties(ZombieData
		SOURCE_DIR
			ZombieData_SOURCE_DIR
	)
endif ()
