# 
# Copyright (c) 2009-2014, Asmodehn's Corp.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
#     * Redistributions of source code must retain the above copyright notice, 
#       this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the 
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Asmodehn's Corp. nor the names of its 
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
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
WkDataDir ( "Resources")
WkDocDir ( "Docs" )

set(BUILD_CppTests OFF CACHE BOOL "CppTests option OFF by default")
set(BUILD_EDITOR_COCOSBUILDER OFF CACHE BOOL "EDITOR_COCOSBUILDER option OFF by default")
set(BUILD_EDITOR_SPINE OFF CACHE BOOL "EDITOR_SPINE option OFF by default")

WkDepends("depends/cocos2d-x")

set(COCOS2D_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/depends/cocos2d-x)
if (WIN32)
include_directories (
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/audio/include
  ${COCOS2D_ROOT}/cocos/2d
  ${COCOS2D_ROOT}/cocos/2d/renderer
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/platform/desktop
  ${COCOS2D_ROOT}/cocos/platform/win32
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

elseif(UNIX AND NOT CMAKE_TOOLCHAIN_FILE) #assumed linux
# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set(ARCH_DIR "64-bit")
else()
	set(ARCH_DIR "32-bit")
endif()
include_directories ( 
  /usr/local/include/GLFW
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/audio/include
  ${COCOS2D_ROOT}/cocos/2d
  ${COCOS2D_ROOT}/cocos/2d/renderer
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/platform/desktop
  ${COCOS2D_ROOT}/cocos/platform/linux
  ${COCOS2D_ROOT}/cocos/base
  ${COCOS2D_ROOT}/cocos/deprecated
  ${COCOS2D_ROOT}/cocos/physics
  ${COCOS2D_ROOT}/cocos/editor-support
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

elseif(CMAKE_TOOLCHAIN_FILE) #assumed Android
include_directories ( 
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/audio/include
  ${COCOS2D_ROOT}/cocos/audio/android
  ${COCOS2D_ROOT}/cocos/2d
  ${COCOS2D_ROOT}/cocos/2d/renderer
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/platform/android
  ${COCOS2D_ROOT}/cocos/base
  ${COCOS2D_ROOT}/cocos/deprecated
  ${COCOS2D_ROOT}/cocos/physics
  ${COCOS2D_ROOT}/cocos/editor-support
  ${COCOS2D_ROOT}/extensions
  ${COCOS2D_ROOT}/external
  ${COCOS2D_ROOT}/external/edtaa3func
  ${COCOS2D_ROOT}/external/jpeg/include/android
  ${COCOS2D_ROOT}/external/png/include/android
  ${COCOS2D_ROOT}/external/tiff/include/android
  ${COCOS2D_ROOT}/external/webp/include/android
  ${COCOS2D_ROOT}/external/curl/include/android
  ${COCOS2D_ROOT}/external/tinyxml2
  ${COCOS2D_ROOT}/external/unzip
  ${COCOS2D_ROOT}/external/sqlite3/include
  ${COCOS2D_ROOT}/external/freetype2/include/android
  ${COCOS2D_ROOT}/external/freetype2/include/android/freetype2
  ${COCOS2D_ROOT}/external/websockets/include/android
  ${COCOS2D_ROOT}/external/spidermonkey/include/android
  ${COCOS2D_ROOT}/external/xxhash
)
link_directories(
  ${COCOS2D_ROOT}/external/png/prebuilt/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/jpeg/prebuilt/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/tiff/prebuilt/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/webp/prebuilt/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/curl/prebuilt/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/sqlite3/libraries/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/freetype2/prebuilt/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/websockets/prebuilt/android/${ANDROID_ABI}
  ${COCOS2D_ROOT}/external/spidermonkey/prebuilt/android/${ANDROID_ABI}
)
endif()


WkAddCFlags("-DCOCOS2D_DEBUG=1")
WkAddCXXFlags("-DCOCOS2D_DEBUG=1")

if(WIN32)
  WkAddCXXFlags("-D_USRDLL -DCOCOS2DXWIN32_EXPORTS -D_WINDOWS -DWIN32")

  if(MSVC)
    WkAddCXXFlags( "-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS" )
    WkAddCXXFlags( "-wd4251 -wd4244 -wd4334 -wd4005 -wd4820 -wd4710 -wd4514 -wd4056 -wd4996 -wd4099" )
    WkAddCXXFlags( "-MP -Qpar" )
  else(MSVC)#MINGW
  
  endif(MSVC)
elseif(APPLE)


else()#Linux

  WkAddCFlags("-DLINUX")
  WkAddCXXFlags("-DLINUX")
endif()


if(NOT MSVC)# all gcc
WkAddCFlags( "-std=c99")
WkAddCXXFlags("-std=c++11")
endif()

if(MINGW)
  WkAddCXXFlags("-DGLEW_STATIC")
endif()

if(USE_CHIPMUNK)
  message(STATUS "Using chipmunk ...")
  WkAddCXXFlags("-DCC_ENABLE_CHIPMUNK_INTEGRATION=1")
elseif(USE_BOX2D)
  message(STATUS "Using box2d ...")
  WkAddCXXFlags("-DCC_ENABLE_BOX2D_INTEGRATION=1")
else(USE_CHIPMUNK)
  message(FATAL_ERROR "Must choose a physics library.")
endif(USE_CHIPMUNK)

WkDepends ("depends/App42_Cocos2DX_SDK")


#The library. reusable for any [Wk]Cocos project.
WkLibraryBuild(WkCocos "Classes/Lib")
  
#The app. Interface to test the library.
WkLibraryBuild(WkCocosApp "Classes/App")
target_link_libraries(WkCocosApp
  WkCocos
)


#Linking with all libraries

if ( BUILD_EDITOR_COCOSBUILDER )
target_link_libraries(WkCocosApp
  cocosbuilder
)
endif()

if ( BUILD_EDITOR_COCOSTUDIO )
target_link_libraries(WkCocosApp
  cocostudio
)
endif()

if ( BUILD_EDITOR_SPINE )
target_link_libraries(WkCocosApp
  spine
)
endif()

if ( BUILD_EXTENSIONS )
target_link_libraries(WkCocosApp
  extensions
)
endif()

if ( USE_BOX2D )
target_link_libraries(WkCocosApp
  box2d
)
endif()

target_link_libraries(WkCocosApp
  audio
  cocos2d
)

#WKCMAKE ISSUE 43 https://github.com/asmodehn/wkcmake/issues/43
# Workaround to actually do the linking
WkExportConfig( ${PROJECT_NAME} )

#Testing
if(WIN32)
    WkTestBuild( maintest win32/main.cpp )
    WkTestRun( WkCocosApp maintest WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_TEST_DIR}/${${PROJECT_NAME}_TEST_DATA_DIR} )
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
			    "Copying ${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt/zlib1.dll to $<TARGET_FILE_DIR:maintest>"
			    VERBATIM)
	ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
				"${COCOS2D_ROOT}/external/curl/prebuilt/win32/libcurl.dll"
				"$<TARGET_FILE_DIR:maintest>"
			    COMMENT
			    "Copying ${COCOS2D_ROOT}/external/curl/prebuilt/win32/libcurl.dll to $<TARGET_FILE_DIR:maintest>"
			    VERBATIM)
	endif ( ${PROJECT_NAME}_ENABLE_TESTS )
elseif (UNIX AND NOT CMAKE_TOOLCHAIN_FILE) #assumed linux
    WkTestBuild( maintest linux/main.cpp )
    WkTestRun( WkCocosApp maintest WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_TEST_DIR}/${${PROJECT_NAME}_TEST_DATA_DIR} )
endif()
