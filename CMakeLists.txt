cmake_minimum_required(VERSION 3.19)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE STRING ""
	)
	message(STATUS "Uses VCPKG CMAKE_TOOLCHAIN_FILE")
endif()

project(Zombie
	DESCRIPTION
		"A 2D Zombie Game"
	VERSION
		0.1.0
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files.
set(SOURCES_ZOMBIE
	# Graphic related
	src/graphics/drawdebugcircle.h

	# Physical
	src/physics/physicalobject.cpp
	src/physics/physicalobject.h
	src/physics/physicengine.cpp
	src/physics/physicengine.h
	src/physics/gameinterface.h
	src/physics/contactlistener.cpp
	src/physics/contactlistener.h
	src/physics/closestraycastcallback.h
	src/physics/inviewquerycallback.h
	src/physics/state.h

	src/physics/stationary/tree.h
	src/physics/stationary/building.h
	src/physics/stationary/buildingproperties.h
	
	src/physics/moving/unit.cpp
	src/physics/moving/unit.h
	src/physics/moving/unitproperties.h
	src/physics/moving/car.cpp
	src/physics/moving/car.h
	src/physics/moving/movingobject.h
	
	src/physics/weapons/gun.cpp
	src/physics/weapons/gun.h
	src/physics/weapons/missilelauncher.cpp
	src/physics/weapons/missilelauncher.h
	src/physics/weapons/missile.cpp
	src/physics/weapons/missile.h
	src/physics/weapons/missileproperties.h
	src/physics/weapons/shot.h
	src/physics/weapons/weapon.h
	src/physics/weapons/weaponproperties.h
	src/physics/weapons/weaponinterface.h
	src/physics/weapons/weaponitem.h

	# Everything
	src/actionhandler.h
	src/auxiliary.cpp
	src/auxiliary.h
	src/box2ddef.h
	src/device.h
	src/configuration.cpp
	src/configuration.h
	src/humanplayer.h
	src/input.h
	src/inputkeyboard.h
	src/logger.cpp
	src/logger.h
	src/main.cpp
	src/player.h
	
	src/zombieplayer.cpp
	src/zombieplayer.h
	src/carplayer.cpp
	src/carplayer.h

	src/zombiegame.cpp
	src/zombiegame.h
	src/zombiewindow.h
	src/zombiewindow.cpp

	# Cmake stuff
	ExternalFetchContent.cmake
	CPack.cmake
)

if (MSVC)
	configure_file(properties.rc ${CMAKE_CURRENT_BINARY_DIR}/properties.rc)
endif ()

find_package(box2d CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt CONFIG REQUIRED)

include(ExternalFetchContent.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS On) 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Zombie)

source_group(TREE
	${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${SOURCES_ZOMBIE}
)

# Copy data to build folder.
if (ZombieData)
	file(COPY ${ZombieData_SOURCE_DIR}/data/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY ${ZombieData_SOURCE_DIR}/data/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 
	file(COPY ${ZombieData_SOURCE_DIR}/data/settings DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 
	file(COPY ${ZombieData_SOURCE_DIR}/data/sounds DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 
endif ()

#file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(ExternalDependencies
	CppSdl2
	ImGui
	Signal
)

if (CppSdl2_Test)
	set(ExternalDependencieTests "${ExternalDependencieTests};CppSdl2_Test")
endif ()
if (CppSdl2_Example)
	set(ExternalDependencieTests "${ExternalDependencieTests};CppSdl2_Example")
endif ()
if (Signal_Test)
	set(ExternalDependencieTests "${ExternalDependencieTests};Signal_Test")
endif ()

set_target_properties(
	${ExternalDependencies}
	${ExternalDependencieTests}
	
	PROPERTIES FOLDER
		ExternalDependencies
)

if (MSVC)
	add_executable(Zombie
		${SOURCES_ZOMBIE}
		zombie_icon.rc
		${CMAKE_CURRENT_BINARY_DIR}/properties.rc
	)
	
	message(STATUS "Hide terminal: -DHideTerminal=1")
	option(HideTerminal "Hide terminal" OFF)
	set_target_properties(Zombie PROPERTIES WIN32_EXECUTABLE ${HideTerminal})

	target_compile_options(Zombie
		PRIVATE
			/W3 /WX /permissive-
			/MP
	)
else ()
	add_executable(Zombie ${SOURCES_ZOMBIE})
	target_compile_options(Zombie
		PRIVATE
			-Wall -pedantic -Wcast-align -Woverloaded-virtual -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function
	)
endif ()

set_target_properties(Zombie
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

target_include_directories(Zombie
	PRIVATE
		src/
)

target_link_libraries(Zombie
	PRIVATE
		box2d::box2d
		nlohmann_json
		nlohmann_json::nlohmann_json
		${ExternalDependencies}
)

extract_git_hash()
create_guid()

target_compile_definitions(Zombie
	PRIVATE
		GIT_VERSION="${GIT_HASH}"
		PROJECT_VERSION="${PROJECT_VERSION}"
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CPack.cmake)
