#/*
#-----------------------------------------------------------------------------
#Filename:    CMakeLists.txt
#-----------------------------------------------------------------------------
#
# A path variable called OGRE_SDK needs to be pointed to the toplevel of an
# SDK build. 
#
# A folder called boost_1_42 needs to be in the top level of the SDK folder.
#
# This presupposes for Linux that the OGRE source has been built and 
# `make install` has been run. It was designed for Ubuntu, so your mileage 
# may vary.
#-----------------------------------------------------------------------------
#*/

cmake_minimum_required(VERSION 2.6)

set(APP_NAME "OgreApp")
set(CONFIGURATION "Debug")

project(${APP_NAME})

# Force an i386 target
set(CMAKE_OSX_ARCHITECTURES "i386")

# Set 10.6 as the base SDK by default
set(XCODE_ATTRIBUTE_SDKROOT macosx10.5)


if ("$ENV{OGRE_SDK}" STREQUAL "")
  message(FATAL_ERROR "The path variable $OGRE_SDK needs to be pointed to the toplevel of the OGRE SDK.")
endif ()

include_directories("./Source")


if (UNIX AND NOT APPLE)
  # Set up includes for client code
  include_directories("/usr/local/include")
  include_directories("/usr/local/include/OGRE")
  include_directories("/usr/local/include/OGRE/Plugins")
  include_directories("/usr/local/include/OGRE/RenderSystems")
  include_directories("/usr/local/include/OGRE/RTShaderSystem")
  include_directories("/usr/local/include/OGRE/Terrain")
  include_directories("/usr/local/include/OGRE/Paging")
  include_directories("/usr/local/include/OGRE/Threading")
endif()

if (APPLE)
  # Set up includes for client code
  include_directories("$ENV{OGRE_SDK}/include")
  include_directories("$ENV{OGRE_SDK}/include/OGRE")
  include_directories("$ENV{OGRE_SDK}/include/OGRE/OSX")
  include_directories("$ENV{OGRE_SDK}/include/OIS")
  include_directories("$ENV{OGRE_SD}/boost_1_42")
endif(APPLE)
 
include_directories("${CMAKE_SOURCE_DIR}/Include")
include_directories("${CMAKE_SOURCE_DIR}/Include/bullet")
 
if(UNIX AND NOT APPLE)
	set(CMAKE_MODULE_PATH "CMake/")
	set(DYNAMIC_LIBRARY_EXT ".so")
endif()
 
if(APPLE)
  set(CMAKE_MODULE_PATH "CMake/")
	set(DYNAMIC_LIBRARY_EXT ".dylib")
endif(APPLE)
 
 
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
 
# #############################################################################
# Find required packages (OGRE, OIS, Boost)
# #############################################################################
 
find_package(OGRE REQUIRED)
 
if(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
  message(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
endif()

find_package(OIS REQUIRED)
 
if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()
 
if (NOT OGRE_BUILD_PLATFORM_IPHONE)
  set(Boost_INCLUDE_DIR "$ENV{OGRE_SDK}/boost_1_42")

	if (WIN32 OR APPLE)
		set(Boost_USE_STATIC_LIBS TRUE)
	else ()
		# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
		set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
	endif ()
	if (MINGW)
		# this is probably a bug in CMake: the boost find module tries to look for
		# boost libraries with name libboost_*, but CMake already prefixes library
		# search names with "lib". This is the workaround.
		set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	endif ()
	set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
	# Components that need linking (NB does not include header-only components like bind)
	set(OGRE_BOOST_COMPONENTS thread date_time regex)
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	if (NOT Boost_FOUND)
		# Try again with the other type of libs
		set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
		find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
	endif()
	
	find_package(Boost QUIET)
  
	# Set up referencing of Boost
	include_directories(${Boost_INCLUDE_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()


 
set(HDRS
	Source/Actions/Action.h
	Source/Actions/ActionPump.h
	Source/Common/Common.h
	Source/Networking/Network.h
)
 
set(SRCS
  Source/main.cpp
  Source/Actions/Action.cpp
  Source/Actions/ActionPump.cpp
  Source/Application/Application.cpp
  Source/Input/Input.cpp
  Source/Graphics/Graphics.cpp  
  Source/Networking/Network.cpp
  
  
)
 
include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
)
 

add_executable(${APP_NAME} WIN32 ${HDRS} ${SRCS})

#set_target_properties(${APP_NAME} PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(${APP_NAME} ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

# post-build copy for win32
#if(WIN32 AND NOT MINGW)
#	add_custom_command( TARGET ${APP_NAME} PRE_BUILD
#		COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
#	add_custom_command( TARGET ${APP_NAME} POST_BUILD
#		COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
#endif(WIN32 AND NOT MINGW)

if(UNIX AND NOT APPLE)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION})
endif(UNIX AND NOT APPLE)

if (APPLE)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION})
endif(APPLE)


if(UNIX AND NOT APPLE)
  set(BUNDLE_LOCATION   "${CMAKE_CURRENT_BINARY_DIR}/bin/${CONFIGURATION}")
  set(BUNDLE_CONTENTS   "${BUNDLE_LOCATION}/Contents")
  set(BUNDLE_PLUGINS    "${BUNDLE_CONTENTS}/Plugins")
  set(BUNDLE_RESOURCES  "${BUNDLE_LOCATION}/Resources")
  set(BUNDLE_MEDIA      "${BUNDLE_CONTENTS}/Media")
endif()


if(APPLE)
	set_property(TARGET ${APP_NAME} PROPERTY MACOSX_BUNDLE TRUE)

  set(BUNDLE_LOCATION   "${CMAKE_CURRENT_BINARY_DIR}/bin/${CONFIGURATION}/${APP_NAME}.app")
  set(BUNDLE_CONTENTS   "${BUNDLE_LOCATION}/Contents")
  set(BUNDLE_PLUGINS    "${BUNDLE_CONTENTS}/Plugins")
  set(BUNDLE_RESOURCES  "${BUNDLE_CONTENTS}/Resources")
  set(BUNDLE_MEDIA      "${BUNDLE_CONTENTS}/Media")
endif(APPLE)


if(UNIX AND NOT APPLE)
 
	install(TARGETS OgreApp
		RUNTIME DESTINATION "bin/${CONFIGURATION}/OgreApp"
		CONFIGURATIONS All
  )
 
  # Copy all the media files into the app bundle
  file(COPY "$ENV{OGRE_SDK}/Samples/Media" DESTINATION "${BUNDLE_CONTENTS}")

  # Copy all the local media files into the app bundle
  file(COPY "${CMAKE_SOURCE_DIR}/Media" DESTINATION "${BUNDLE_CONTENTS}")

  # Copy the local game resources to the bundle
  file(GLOB configs "$ENV{CMAKE_CURRENT_BINARY_DIR}UnixResources/*.cfg")
  
  foreach(config ${configs})
    file(COPY ${config} DESTINATION "${BUNDLE_LOCATION}")
  endforeach(config)
  
  # Copy all the dynamic libraries to the Plugins folder in the app.
  file(GLOB plugins "/usr/local/lib/OGRE/*.so")
  file(MAKE_DIRECTORY "${BUNDLE_PLUGINS}")

  foreach(plugin ${plugins})
    file(COPY ${plugin} DESTINATION "${BUNDLE_PLUGINS}")
  endforeach(plugin)

endif()











#
# Apple stuff
# 



if(APPLE)

	install(TARGETS OgreApp
	  BUNDLE DESTINATION "."
		RUNTIME DESTINATION bin/${CONFIGURATION}
		CONFIGURATIONS All
  )

  # Copy all the media files into the app bundle from the SDK
  file(COPY "$ENV{OGRE_SDK}/Media" DESTINATION "${BUNDLE_CONTENTS}")

  # Copy all the local media files into the app bundle
  file(COPY "$ENV{CMAKE_CURRENT_BINARY_DIR}Media" DESTINATION "${BUNDLE_CONTENTS}")

  # Copy the local game resources to the bundle
  file(COPY "$ENV{CMAKE_CURRENT_BINARY_DIR}Resources" DESTINATION "${BUNDLE_CONTENTS}")

  # Copy all the dynamic libraries to the Plugins folder in the app.
  file(GLOB plugins "$ENV{OGRE_SDK}/lib/*.dylib")
  file(MAKE_DIRECTORY "${BUNDLE_PLUGINS}")

  foreach(plugin ${plugins})
    file(COPY ${plugin} DESTINATION "${BUNDLE_PLUGINS}")
  endforeach(plugin)

endif(APPLE)