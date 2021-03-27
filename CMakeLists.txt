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
	src/Graphic/graphic.cpp
	src/Graphic/graphic.h

	# Physical objects
	src/Physic/physicalobject.cpp
	src/Physic/physicalobject.h
	src/Physic/contactlistener.cpp
	src/Physic/contactlistener.h
	src/Physic/closestraycastcallback.h
	src/Physic/inviewquerycallback.h
	src/Physic/state.h

	src/Physic/Stationary/tree.h
	src/Physic/Stationary/building.h
	src/Physic/Stationary/buildingproperties.h
	
	src/Physic/Moving/unit.cpp
	src/Physic/Moving/unit.h
	src/Physic/Moving/unitproperties.h
	src/Physic/Moving/car.cpp
	src/Physic/Moving/car.h
	src/Physic/Moving/movingobject.h
	
	# Weapons
	src/Weapons/missilelauncher.h
	src/Weapons/missilelauncher.cpp
	src/Weapons/missile.cpp
	src/Weapons/missile.h
	src/Weapons/missileproperties.h
	src/Weapons/shot.h
	src/Weapons/weapon.h
	src/Weapons/weaponproperties.h
	src/Weapons/weaponinterface.h
	src/Weapons/weaponitem.h

	# Everything
	src/actionhandler.h
	src/auxiliary.cpp
	src/auxiliary.h
	src/box2ddef.h
	src/device.h
	src/gameinterface.h
	src/gamedata.cpp
	src/gamedata.h
	src/Weapons/gun.cpp
	src/Weapons/gun.h
	src/humanplayer.h
	src/input.h
	src/inputkeyboard.h
	src/logger.cpp
	src/logger.h
	src/main.cpp
	src/player.h
	
	src/gameobject.cpp
	src/gameobject.h
	src/survivorbehavior.cpp
	src/survivorbehavior.h
	src/zombiebehavior.cpp
	src/zombiebehavior.h
	src/physicengine.cpp
	src/physicengine.h
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

file(COPY data/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.

set(ExternalDependencies
	CppSdl2
	ImGui
)

if (CppSdl2_Test)
	set(ExternalDependencieTests "${ExternalDependencieTests};CppSdl2_Test")
endif ()
if (CppSdl2_Example)
	set(ExternalDependencieTests "${ExternalDependencieTests};CppSdl2_Example")
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
			"/permissive-"
	)

else ()
	add_executable(Zombie ${SOURCES_ZOMBIE})
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
