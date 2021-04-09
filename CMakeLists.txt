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
	

	# Physical objects
	src/physics/physicalobject.cpp
	src/physics/physicalobject.h
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
	
	# Weapons
	src/weapons/gun.cpp
	src/weapons/gun.h
	src/weapons/missilelauncher.cpp
	src/weapons/missilelauncher.h
	src/weapons/missile.cpp
	src/weapons/missile.h
	src/weapons/missileproperties.h
	src/weapons/shot.h
	src/weapons/weapon.h
	src/weapons/weaponproperties.h
	src/weapons/weaponinterface.h
	src/weapons/weaponitem.h

	# Everything
	src/actionhandler.h
	src/auxiliary.cpp
	src/auxiliary.h
	src/box2ddef.h
	src/device.h
	src/gameinterface.h
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
			"/permissive-"
			"/MP"
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
