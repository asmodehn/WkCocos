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

if (WIN32 AND MSVC)
	#visual only allows debugging 4096 objects maximum...
	WkRmFlags(CMAKE_C_FLAGS_DEBUG "/Zi")
	WkRmFlags(CMAKE_CXX_FLAGS_DEBUG "/Zi")
endif()

#Project Declaration
WkProject( WkCocos CXX )

#Configuration of custom directories
WkIncludeDir ( "Include" )
WkTestDir ( "Tests" )
WkDataDir ( "Resources")
WkDocDir ( "Docs" )

add_subdirectory("depends/entityx")
include_directories( "depends/entityx")

add_subdirectory("depends/unittest-cpp")
include_directories( "depends/unittest-cpp")

# setting include directory for upper project to include it
set( WkCocos_INCLUDE_DIRS ${WkCocos_INCLUDE_DIRS} "depends/unittest-cpp")

# setting include directory for upper project to include it
set( WkCocos_INCLUDE_DIRS ${WkCocos_INCLUDE_DIRS} "depends/entityx")

set(BUILD_CppTests OFF CACHE BOOL "CppTests option OFF by default")
set(BUILD_EDITOR_COCOSBUILDER OFF CACHE BOOL "EDITOR_COCOSBUILDER option OFF by default")
set(BUILD_EDITOR_SPINE OFF CACHE BOOL "EDITOR_SPINE option OFF by default")
set(BUILD_LIBS_LUA OFF CACHE BOOL "BUILD_LIBS_LUA option OFF by default")
set(BUILD_LuaTests OFF CACHE BOOL "BUILD_LuaTests option OFF by default")

WkDepends("depends/cocos2d-x")

set(COCOS2D_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/depends/cocos2d-x)

# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "64-bit")
else()
set(ARCH_DIR "32-bit")
endif()

if(WIN32) # Win32
  set(PLATFORM_FOLDER win32)
elseif(APPLE)# osx or ios
  set(PLATFORM_FOLDER mac)
else() # Assume Linux
  set(PLATFORM_FOLDER linux)
endif()

include_directories(
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/platform/desktop
  ${COCOS2D_ROOT}/cocos/platform/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/cocos/audio/include
  ${COCOS2D_ROOT}/cocos/editor-support
  ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/auto
  ${COCOS2D_ROOT}/cocos/scripting/lua-bindings/manual
  ${COCOS2D_ROOT}/extensions
  ${COCOS2D_ROOT}/external
  ${COCOS2D_ROOT}/external-bin
  ${COCOS2D_ROOT}/external-bin/tinyxml2
  ${COCOS2D_ROOT}/external-bin/unzip
  ${COCOS2D_ROOT}/external-bin/json
  ${COCOS2D_ROOT}/external-bin/sqlite3/include
  ${COCOS2D_ROOT}/external-bin/edtaa3func
  ${COCOS2D_ROOT}/external/chipmunk/include/chipmunk
  ${COCOS2D_ROOT}/external-bin/jpeg/include/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/external-bin/png/include/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/external-bin/tiff/include/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/external-bin/webp/include/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/external-bin/glfw3/include/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/external-bin/freetype2/include/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/external-bin/websockets/include/${PLATFORM_FOLDER}
  ${COCOS2D_ROOT}/external-bin/xxhash
  ${COCOS2D_ROOT}/external/xxtea
)

if (WIN32)
include_directories (
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/gles/include/OGLES
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/icon/include
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/zlib/include
    ${COCOS2D_ROOT}/external-bin/curl/include/${PLATFORM_FOLDER}
  )
  if(NOT MINGW)
  link_directories(

    ${COCOS2D_ROOT}/external-bin/glfw3/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/sqlite3/libraries/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/jpeg/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/tiff/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/png/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/freetype2/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/curl/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/websockets/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/webp/prebuilt/${PLATFORM_FOLDER}
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/icon/prebuilt
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/zlib/prebuilt
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/gles/prebuilt

  )
  endif()
#link_directories(
#)
elseif(APPLE)
  include_directories(
  	${COCOS2D_ROOT}/external-bin/curl/include/${PLATFORM_FOLDER}
  )
elseif(UNIX AND NOT CMAKE_TOOLCHAIN_FILE) #assumed linux
include_directories (
    /usr/include
    /usr/include/GLFW
    /usr/local/include/GLFW
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/fmod/include/${ARCH_DIR}
  )
link_directories(
    /usr/local/lib
    ${COCOS2D_ROOT}/external-bin/jpeg/prebuilt/${PLATFORM_FOLDER}/${ARCH_DIR}
    ${COCOS2D_ROOT}/external-bin/tiff/prebuilt/${PLATFORM_FOLDER}/${ARCH_DIR}
    ${COCOS2D_ROOT}/external-bin/webp/prebuilt/${PLATFORM_FOLDER}/${ARCH_DIR}
    ${COCOS2D_ROOT}/external-bin/png/prebuilt/${PLATFORM_FOLDER}/${ARCH_DIR}
    ${COCOS2D_ROOT}/external-bin/freetype2/prebuilt/${PLATFORM_FOLDER}/${ARCH_DIR}
    ${COCOS2D_ROOT}/external-bin/websockets/prebuilt/${PLATFORM_FOLDER}/${ARCH_DIR}
    ${COCOS2D_ROOT}/external-bin/${PLATFORM_FOLDER}-specific/fmod/prebuilt/${ARCH_DIR}
  )

elseif(CMAKE_TOOLCHAIN_FILE) #assumed Android
#include_directories (
#  ${COCOS2D_ROOT}/cocos/audio/android
#  ${COCOS2D_ROOT}/cocos/2d
#  ${COCOS2D_ROOT}/cocos/2d/renderer
#  ${COCOS2D_ROOT}/cocos/platform
#  ${COCOS2D_ROOT}/cocos/platform/android
#  ${COCOS2D_ROOT}/cocos/base
#  ${COCOS2D_ROOT}/cocos/deprecated
#  ${COCOS2D_ROOT}/cocos/physics
#  ${COCOS2D_ROOT}/external-bin/edtaa3func
#  ${COCOS2D_ROOT}/external-bin/jpeg/include/android
#  ${COCOS2D_ROOT}/external-bin/png/include/android
#  ${COCOS2D_ROOT}/external-bin/tiff/include/android
#  ${COCOS2D_ROOT}/external-bin/webp/include/android
#  ${COCOS2D_ROOT}/external-bin/curl/include/android
#  ${COCOS2D_ROOT}/external-bin/sqlite3/include
#  ${COCOS2D_ROOT}/external-bin/freetype2/include/android
#  ${COCOS2D_ROOT}/external-bin/freetype2/include/android/freetype2
#  ${COCOS2D_ROOT}/external-bin/websockets/include/android
#  ${COCOS2D_ROOT}/external/spidermonkey/include/android
#)
#link_directories(
#  ${COCOS2D_ROOT}/external-bin/png/prebuilt/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external-bin/jpeg/prebuilt/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external-bin/tiff/prebuilt/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external-bin/webp/prebuilt/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external-bin/curl/prebuilt/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external-bin/sqlite3/libraries/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external-bin/freetype2/prebuilt/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external-bin/websockets/prebuilt/android/${ANDROID_ABI}
#  ${COCOS2D_ROOT}/external/spidermonkey/prebuilt/android/${ANDROID_ABI}
#)
endif()


WkAddCFlags("-DCOCOS2D_DEBUG=1")
WkAddCXXFlags("-DCOCOS2D_DEBUG=1")

WkAddCFlags("-DSOOMLA_DEBUG=1")
WkAddCXXFlags("-DSOOMLA_DEBUG=1")

if(WIN32)
  WkAddCXXFlags("-DIGNORE_EXPORT -D_WINDOWS -DWIN32")
  if(MSVC)
    WkAddCXXFlags( "-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS" )
    WkAddCXXFlags( "-wd4251 -wd4334 -wd4820 -wd4710 -wd4514 -wd4056 -wd4996 -wd4099 -wd4819" )
    WkAddCXXFlags( "-MP -Qpar" )
    WkAddCXXFlags( "-Zi" )
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
  WkAddCXXFlags("-DCC_ENABLE_CHIPMUNK_INTEGRATION=1")
elseif(USE_BOX2D)
  WkAddCXXFlags("-DCC_ENABLE_BOX2D_INTEGRATION=1")
else(USE_CHIPMUNK)
  message(FATAL_ERROR "Must choose a physics library.")
endif(USE_CHIPMUNK)

WkDepends ("depends/App42_Cocos2DX_SDK")

WkDepends ("depends/cocos2dx-store")
#Temporary work around wkcmake bug with dependencies at multiple levels
set ( WkCocos_Cocos2dxStore_LIBRARIES Cocos2dxStore ${Cocos2dxStore_SoomlaCocos2dxCore_LIBRARIES} CACHE PATH "Cocos2dxStore Libraries")

set( WkCocos_INCLUDE_DIRS ${WkCocos_INCLUDE_DIRS} ${WkCocos_App42_INCLUDE_DIRS} ${WkCocos_Cocos2dxStore_INCLUDE_DIRS})

#The library. reusable for any [Wk]Cocos project.
WkLibraryBuild(WkCocos "Classes/Lib")

set( WkCocos_INCLUDE_DIRS ${WkCocos_INCLUDE_DIRS}
	CACHE PATH "Headers Directories"
)

#Add WkCocosApp only if we are the top project
if ( ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME} )
	#The app. Interface to test the library.
	WkLibraryBuild(WkCocosApp "Classes/App")
	target_link_libraries(WkCocosApp
	  WkCocos
	  entityx  # needed because entityx is not a wkcmake project
	)

	if ( WIN32)
	target_link_libraries(WkCocosApp
	  Rpcrt4.lib #needed for UUID computation on Win32 only
	)
	elseif (UNIX)
	target_link_libraries(WkCocosApp
	  uuid #needed for UUID computation on Linux only
	)
	endif()


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

	#Testing on by default.
	set(WkCocos_ENABLE_TESTS ON CACHE BOOL "Wether or not you want the project to include the tests and enable automatic testing for WkCocos")


	if ( ${PROJECT_NAME}_ENABLE_TESTS ) # if the target has been defined ( tests enabled )
		#Main test
		if(WIN32)
		WkTestBuild( maintest win32/main.cpp )
		WkTestRun( WkCocosApp maintest WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_TEST_DIR}/${${PROJECT_NAME}_TEST_DATA_DIR} )
		#also copying dlls to binary directory for the executable to run
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:cocos2d>"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying $<TARGET_FILE:cocos2d> to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:cocosui>"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying $<TARGET_FILE:cocosui> to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:cocostudio>"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying $<TARGET_FILE:cocostudio> to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/win32-specific/gles/prebuilt/glew32.dll"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/win32-specific/gles/prebuilt/glew32.dll to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/win32-specific/zlib/prebuilt/zlib1.dll"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/win32-specific/zlib/prebuilt/zlib1.dll to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/curl/prebuilt/win32/libcurl.dll"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/curl/prebuilt/win32/libcurl.dll to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/sqlite3/libraries/win32/sqlite3.dll"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/sqlite3/libraries/win32/sqlite3.dll to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET maintest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/websockets/prebuilt/win32/websockets.dll"
					"$<TARGET_FILE_DIR:maintest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/websockets/prebuilt/win32/websockets.dll to $<TARGET_FILE_DIR:maintest>"
					VERBATIM)

		elseif (UNIX AND NOT CMAKE_TOOLCHAIN_FILE) #assumed linux
			WkTestBuild( maintest linux/main.cpp )
			WkTestRun( WkCocosApp maintest WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_TEST_DIR}/${${PROJECT_NAME}_TEST_DATA_DIR} )
		endif()

		#Auto tests

		# setting include directory for upper project to include it
		set( WkCocos_INCLUDE_DIRS ${WkCocos_INCLUDE_DIRS} "depends/unittest-cpp")

		WkTestBuild( autotest AutoTests/main.cpp AutoTests/Save.cpp AutoTests/StrongBox.cpp AutoTests/Actor.cpp)
		target_link_libraries(autotest UnitTest++ )
		WkTestRun( WkCocosApp autotest WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/${${PROJECT_NAME}_TEST_DIR}/${${PROJECT_NAME}_TEST_DATA_DIR} )

		if(WIN32)
		#also copying dlls to binary directory for the executable to run
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:cocos2d>"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying $<TARGET_FILE:cocos2d> to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:cocosui>"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying $<TARGET_FILE:cocosui> to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"$<TARGET_FILE:cocostudio>"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying $<TARGET_FILE:cocostudio> to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/win32-specific/gles/prebuilt/glew32.dll"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/win32-specific/gles/prebuilt/glew32.dll to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/win32-specific/zlib/prebuilt/zlib1.dll"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/win32-specific/zlib/prebuilt/zlib1.dll to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/curl/prebuilt/win32/libcurl.dll"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/curl/prebuilt/win32/libcurl.dll to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/sqlite3/libraries/win32/sqlite3.dll"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/sqlite3/libraries/win32/sqlite3.dll to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		ADD_CUSTOM_COMMAND( TARGET autotest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
					"${COCOS2D_ROOT}/external-bin/websockets/prebuilt/win32/websockets.dll"
					"$<TARGET_FILE_DIR:autotest>"
					COMMENT
					"Copying ${COCOS2D_ROOT}/external-bin/websockets/prebuilt/win32/websockets.dll to $<TARGET_FILE_DIR:autotest>"
					VERBATIM)
		endif(WIN32)
	endif ( ${PROJECT_NAME}_ENABLE_TESTS )

endif(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})

