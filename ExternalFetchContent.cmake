include(FetchContent)
# Load external github projects
FetchContent_Declare(CppSdl2
	GIT_REPOSITORY
		https://github.com/mwthinker/CppSdl2.git
	GIT_TAG
		d0f488d851cf294a616058b093176982c4e64724
)
FetchContent_MakeAvailable(CppSdl2)

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
