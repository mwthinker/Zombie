
install(TARGETS Zombie
	RUNTIME
		DESTINATION .
	COMPONENT application
)

install(DIRECTORY ${Zombie_SOURCE_DIR}/fonts/
	DESTINATION .
	COMPONENT data
)

install(DIRECTORY ${Zombie_SOURCE_DIR}/images/
	DESTINATION .
	COMPONENT data
)

if (MSVC)
	# Tell CMake to install the windows runtime libraries to the programs
	# directory and tell CPack that they belong to the "applications" component
	set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
	include(InstallRequiredSystemLibraries)
	file(GLOB DLL_FILES_DEBUG "${CMAKE_BINARY_DIR}/Debug/*.dll")
	file(GLOB DLL_FILES_MINSIZEREL "${CMAKE_BINARY_DIR}/MinSizeRel/*.dll")
	file(GLOB DLL_FILES_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/RelWithDebInfo/*.dll")
	file(GLOB DLL_FILES_RELEASE "${CMAKE_BINARY_DIR}/Release/*.dll")

	set(DLL_FILES "$<$<CONFIG:Debug>:${DLL_FILES_DEBUG}>")
	set(DLL_FILES "${DLL_FILES}$<$<CONFIG:MinSizeRel>:${DLL_FILES_MINSIZEREL}>")
	set(DLL_FILES "${DLL_FILES}$<$<CONFIG:RelWithDebInfo>:${DLL_FILES_RELWITHDEBINFO}>")
	set(DLL_FILES "${DLL_FILES}$<$<CONFIG:Release>:${DLL_FILES_RELEASE}>")

	install(PROGRAMS "${DLL_FILES}"
		DESTINATION .
		COMPONENT application
	)
endif ()

set(CPACK_PACKAGE_CONTACT "mwthinker@yahoo.com")

# CPack configuration
set(CPACK_PACKAGE_VENDOR "MW")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Installs Zombie")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Zombie")	

set(CPACK_GENERATOR ZIP)

# Tell CPack about the components and group the data components together.
if (MSVC)
	set(CPACK_COMPONENTS_ALL application data runtime vcredist)
else ()
	set(CPACK_COMPONENTS_ALL application data)
endif ()

# More descriptive names for each of the components, and component groups.
set(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "Application")
set(CPACK_COMPONENT_APPLICATION_REQUIRED true)

set(CPACK_COMPONENT_DATA_DISPLAY_NAME "Data")
set(CPACK_COMPONENT_DATA_REQUIRED true)

set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime")
set(CPACK_COMPONENT_RUNTIME_REQUIRED true)

# Text from "LICENSE.txt" is displayed in the installer's license tab.
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
#set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\data\\\\package\\\\mw.bmp")

set(CPACK_PACKAGE_EXECUTABLES "Zombie" "Zombie")
set(CPACK_CREATE_DESKTOP_LINKS "Zombie")

include(CPack)
