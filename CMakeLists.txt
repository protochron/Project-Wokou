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
set(XCODE_ATTRIBUTE_SDKROOT macosx10.6)

set(CMAKE_OSX_SYSROOT "/")

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
  include_directories("$ENV{OGRE_SDK}/boost_1_42")

  set(BULLET_ROOT "/usr/local")
endif()

if (APPLE)
  # Set up includes for client code
  include_directories("$ENV{OGRE_SDK}/include")
  include_directories("$ENV{OGRE_SDK}/include/OGRE")
  include_directories("$ENV{OGRE_SDK}/include/OGRE/OSX")
  include_directories("$ENV{OGRE_SDK}/include/OIS")
#  include_directories("$ENV{OGRE_SDK}/boost_1_42")
endif(APPLE)
 
include_directories("${CMAKE_SOURCE_DIR}/Include")
 
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
 
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package( Boost 1.42.0 COMPONENTS system date_time filesystem thread )

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  #link_directories( ${Boost_LIBRARY_DIRS} )
  #link_libraries(${Boost_LIBRARIES})
endif(Boost_FOUND)
 
# #############################################################################
# Find Bullet
# #############################################################################

find_package(Bullet COMPONENTS Dynamics REQUIRED )

if (BULLET_FOUND)
  include_directories("${BULLET_INCLUDE_DIRS}")
endif(BULLET_FOUND)
 

# #############################################################################
# Find CEGUI
# #############################################################################

find_package(CEGUI REQUIRED)

if (CEGUI_FOUND)
  message(STATUS ">>>>>>> ${CEGUI_INCLUDE_DIR}")
  include_directories("${CEGUI_INCLUDE_DIR}")
endif(CEGUI_FOUND)
 
set(HDRS
	Source/Actions/Action.h
	Source/Actions/ActionPump.h
	Source/Common/Common.h
	Source/Networking/Network.h
    Source/Terrain/Terrain.h
    Source/Physics/Physics.h
)
 
set(SRCS
  Source/main.cpp
  Source/Actions/Action.cpp
  Source/Actions/ActionPump.cpp
  Source/Application/Application.cpp
  Source/Engine/Engine.cpp
  Source/Input/Input.cpp
  Source/Graphics/Graphics.cpp  
  Source/Networking/Network.cpp
  Source/Terrain/Terrain.cpp
  Source/Physics/Physics.cpp
)
 
include_directories( ${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	${BULLET_INCLUDE_DIRS}
)
 


add_executable(${APP_NAME} ${HDRS} ${SRCS})

#set_target_properties(${APP_NAME} PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(${APP_NAME} ${OGRE_LIBRARIES} ${OIS_LIBRARIES} 
                                  ${OGRE_Terrain_LIBRARIES} ${BULLET_LIBRARIES}
                                    ${CEGUI_LIBRARIES} ${CEGUI_OGRE_LIBRARIES}
                                  )

if (Boost_FOUND)
#  link_directories( ${Boost_LIBRARY_DIRS} )
  target_link_libraries(${APP_NAME} ${Boost_LIBRARIES})
endif(Boost_FOUND)

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
  set(BUNDLE_COMPONENTS "${BUNDLE_CONTENTS}/Components")
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

  # Copy all in the local Plugins folder into the bundle
  file(GLOB ps "$ENV{CMAKE_CURRENT_BINARY_DIR}/Plugins")
  
  foreach(p ${ps})
    file(COPY ${p} DESTINATION "${BUNDLE_PLUGINS}")
  endforeach(p)


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
  file(GLOB plugins "$ENV{OGRE_SDK}/lib/Plugin_*.dylib")
  file(MAKE_DIRECTORY "${BUNDLE_PLUGINS}")

  foreach(plugin ${plugins})
    file(COPY ${plugin} DESTINATION "${BUNDLE_PLUGINS}")
  endforeach(plugin)

  # Make sure to pick up RenderSystem_GL
  file(COPY "$ENV{OGRE_SDK}/lib/RenderSystem_GL.dylib" DESTINATION "${BUNDLE_PLUGINS}")

  # Copy all of the dynamic library components into the app
  file(GLOB libs "$ENV{OGRE_SDK}/lib/lib*.dylib")
  file(MAKE_DIRECTORY "${BUNDLE_COMPONENTS}")

  foreach(lib ${libs})
    file(COPY ${lib} DESTINATION "${BUNDLE_COMPONENTS}")
  endforeach(lib)

  # Copy all in the local Plugins folder into the bundle
  file(GLOB ps "$ENV{CMAKE_CURRENT_BINARY_DIR}Plugins/*")
  
  foreach(p ${ps})
    file(COPY ${p} DESTINATION "${BUNDLE_PLUGINS}")
  endforeach(p)
  
endif(APPLE)
