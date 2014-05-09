# 
# Copyright (c) 2009-2014, Asmodehn's Corp.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
#     * Redistributions of source code must retain the above copyright notice, 
#	    this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#		notice, this list of conditions and the following disclaimer in the 
#	    documentation and/or other materials provided with the distribution.
#     * Neither the name of the Asmodehn's Corp. nor the names of its 
#	    contributors may be used to endorse or promote products derived
#	    from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#

#Mandatory commands to use wkcmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE ( "CMake/WkCMake.cmake" )
WkCMakeDir( "CMake" )

#Project Declaration
WkProject( WkCocos CXX )

#Configuration of custom directories
WkIncludeDir ( "Include" )
WkTestDir ( "Tests" )
WkTestDataDir ( "Resources")
WkDocDir ( "Docs" )

#Build rules
option(USE_CHIPMUNK "Use chipmunk for physics library" ON)
option(USE_BOX2D "Use box2d for physics library" OFF)

if (MSVC)
	set(CMAKE_C_FLAGS_DEBUG "-DCOCOS2D_DEBUG=1")
	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
	set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

endif()

if(USE_CHIPMUNK)
  message("Using chipmunk ...")
  add_definitions(-DCC_ENABLE_CHIPMUNK_INTEGRATION=1)
  if(UNIX) # assuming linux
    add_definitions(-DLINUX )
  endif()
elseif(USE_BOX2D)
  message("Using box2d ...")
  add_definitions(-DCC_ENABLE_BOX2D_INTEGRATION=1)
  if(UNIX) # assuming linux
    add_definitions(-DLINUX )
  endif()
else(USE_CHIPMUNK)
  message(FATAL_ERROR "Must choose a physics library.")
endif(USE_CHIPMUNK)

# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "64-bit")
else()
set(ARCH_DIR "32-bit")
endif()

set(COCOS2D_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/depends/cocos2d-x)
if (WIN32)
include_directories (
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/audio/include
  ${COCOS2D_ROOT}/cocos/2d
  ${COCOS2D_ROOT}/cocos/2d/renderer
  ${COCOS2D_ROOT}/cocos/2d/platform
  ${COCOS2D_ROOT}/cocos/2d/platform/desktop
  ${COCOS2D_ROOT}/cocos/2d/platform/win32
  ${COCOS2D_ROOT}/cocos/base
  ${COCOS2D_ROOT}/cocos/deprecated
  ${COCOS2D_ROOT}/cocos/physics
  ${COCOS2D_ROOT}/cocos/editor-support
  ${COCOS2D_ROOT}/cocos/math
  ${COCOS2D_ROOT}/extensions
  ${COCOS2D_ROOT}/external
  ${COCOS2D_ROOT}/external/edtaa3func
  ${COCOS2D_ROOT}/external/jpeg/include/win32
  ${COCOS2D_ROOT}/external/png/include/win32
  ${COCOS2D_ROOT}/external/tiff/include/win32
  ${COCOS2D_ROOT}/external/webp/include/win32
  ${COCOS2D_ROOT}/external/curl/include/win32
  ${COCOS2D_ROOT}/external/tinyxml2
  ${COCOS2D_ROOT}/external/unzip
  ${COCOS2D_ROOT}/external/sqlite3/include
  ${COCOS2D_ROOT}/external/chipmunk/include/chipmunk
  ${COCOS2D_ROOT}/external/freetype2/include/win32
  ${COCOS2D_ROOT}/external/websockets/include/win32
  ${COCOS2D_ROOT}/external/spidermonkey/include/win32
  ${COCOS2D_ROOT}/external/glfw3/include/win32
  ${COCOS2D_ROOT}/external/win32-specific/gles/include/OGLES
  ${COCOS2D_ROOT}/external/win32-specific/icon/include
  ${COCOS2D_ROOT}/external/win32-specific/zlib/include
  ${COCOS2D_ROOT}/external/xxhash
)
link_directories(
  /usr/local/lib
  ${COCOS2D_ROOT}/external/png/prebuilt/win32
  ${COCOS2D_ROOT}/external/jpeg/prebuilt/win32
  ${COCOS2D_ROOT}/external/tiff/prebuilt/win32
  ${COCOS2D_ROOT}/external/glfw3/prebuilt/win32
  ${COCOS2D_ROOT}/external/webp/prebuilt/win32
  ${COCOS2D_ROOT}/external/curl/prebuilt/win32
  ${COCOS2D_ROOT}/external/sqlite3/libraries/win32
  ${COCOS2D_ROOT}/external/freetype2/prebuilt/win32
  ${COCOS2D_ROOT}/external/websockets/prebuilt/win32
  ${COCOS2D_ROOT}/external/spidermonkey/prebuilt/win32
  ${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt
  ${COCOS2D_ROOT}/external/win32-specific/icon/prebuilt
  ${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt
)

elseif(UNIX) #assumed linux
include_directories ( 
  /usr/local/include/GLFW
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/audio/include
  ${COCOS2D_ROOT}/cocos/2d
  ${COCOS2D_ROOT}/cocos/2d/renderer
  ${COCOS2D_ROOT}/cocos/2d/platform
  ${COCOS2D_ROOT}/cocos/2d/platform/desktop
  ${COCOS2D_ROOT}/cocos/2d/platform/linux
  ${COCOS2D_ROOT}/cocos/base
  ${COCOS2D_ROOT}/cocos/deprecated
  ${COCOS2D_ROOT}/cocos/physics
  ${COCOS2D_ROOT}/cocos/editor-support
  ${COCOS2D_ROOT}/cocos/math
  ${COCOS2D_ROOT}/extensions
  ${COCOS2D_ROOT}/external
  ${COCOS2D_ROOT}/external/edtaa3func
  ${COCOS2D_ROOT}/external/jpeg/include/linux
  ${COCOS2D_ROOT}/external/tiff/include/linux
  ${COCOS2D_ROOT}/external/webp/include/linux
  ${COCOS2D_ROOT}/external/tinyxml2
  ${COCOS2D_ROOT}/external/unzip
  ${COCOS2D_ROOT}/external/freetype2/include/linux
  ${COCOS2D_ROOT}/external/websockets/include/linux
  ${COCOS2D_ROOT}/external/spidermonkey/include/linux
  ${COCOS2D_ROOT}/external/linux-specific/fmod/include/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/xxhash
)
link_directories(
  /usr/local/lib
  ${COCOS2D_ROOT}/external/jpeg/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/tiff/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/webp/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/freetype2/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/websockets/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/spidermonkey/prebuilt/linux/${ARCH_DIR}
  ${COCOS2D_ROOT}/external/linux-specific/fmod/prebuilt/${ARCH_DIR}
)
endif()

# math
add_subdirectory(${COCOS2D_ROOT}/cocos/math)

if(USE_CHIPMUNK)
	include_directories(${COCOS2D_ROOT}/external/chipmunk/include/chipmunk)
	# chipmunk library
	add_subdirectory(${COCOS2D_ROOT}/external/chipmunk/src)
endif()

if(USE_BOX2D)
	# box2d library
	add_subdirectory(${COCOS2D_ROOT}/external/Box2D)
endif()

# unzip library
add_subdirectory(${COCOS2D_ROOT}/external/unzip)

# tinyxml2 library
add_subdirectory(${COCOS2D_ROOT}/external/tinyxml2)

# audio
add_subdirectory(${COCOS2D_ROOT}/cocos/audio)

# xxhash library
add_subdirectory(${COCOS2D_ROOT}/external/xxhash)

# cocos base library
add_subdirectory(${COCOS2D_ROOT}/cocos/base)

# cocos 2d library
add_subdirectory(${COCOS2D_ROOT}/cocos/2d)

# cocos storage
add_subdirectory(${COCOS2D_ROOT}/cocos/storage)

# ui
add_subdirectory(${COCOS2D_ROOT}/cocos/ui)

# network
add_subdirectory(${COCOS2D_ROOT}/cocos/network)

# extensions
add_subdirectory(${COCOS2D_ROOT}/extensions)

## Editor Support

# spine
add_subdirectory(${COCOS2D_ROOT}/cocos/editor-support/spine)

# cocosbuilder
add_subdirectory(${COCOS2D_ROOT}/cocos/editor-support/cocosbuilder)

# cocostudio
add_subdirectory(${COCOS2D_ROOT}/cocos/editor-support/cocostudio)

if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(FMOD_LIB "fmodex64")
else()
set(FMOD_LIB "fmodex")
endif()

WkLibraryBuild(WkCocos "Classes")
  
#Linking with all libraries
target_link_libraries(WkCocos 
  cocosbase
  cocosmath
  chipmunk_static
  tinyxml2
  unzip
  xxhash
  ui
  network
  storage
  spine
  cocostudio
  cocosbuilder
  extensions
  audio
  cocos2d
  )

if (MSVC)
	WkAddCDefinitions(Debug "COCOS2D_DEBUG=1")
	WkAddCXXDefinitions(Debug "COCOS2D_DEBUG=1")

	WkAddCFlags("-D_CRT_SECURE_NO_WARNINGS")
	WkAddCXXFlags("-D_CRT_SECURE_NO_WARNINGS")

elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    WkAddCDefinitions(Debug "COCOS2D_DEBUG=1")
    WkAddCXXDefinitions(Debug "COCOS2D_DEBUG=1")

	WkAddCFlags("-std=c99")
	WkAddCXXFlags("-std=c++11")

endif()

#Testing
if(WIN32)
    WkTestBuild( maintest win32/main.cpp )
    WkTestRun( WkCocos maintest WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_TEST_DIR}/${${PROJECT_NAME}_TEST_DATA_DIR} )
	if ( ${PROJECT_NAME}_ENABLE_TESTS ) # if the target has been defined ( tests enbled )
	#also copying dlls to binary directory for the executable to run
	ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
				"${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt/glew32.dll"
				"$<TARGET_FILE_DIR:maintest>"
				COMMENT
			    "Copying ${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt/glew32.dll to $<TARGET_FILE_DIR:maintest>"
			    VERBATIM)
	ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
				"${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt/zlib1.dll"
				"$<TARGET_FILE_DIR:maintest>"
			    COMMENT
			    "Copying ${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt/glew32.dll to $<TARGET_FILE_DIR:maintest>"
			    VERBATIM)
	endif ( ${PROJECT_NAME}_ENABLE_TESTS )
elseif (UNIX) #assumed linux
    WkTestBuild( maintest linux/main.cpp )
    WkTestRun( WkCocos maintest WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_TEST_DIR}/${${PROJECT_NAME}_TEST_DATA_DIR} )
endif()
