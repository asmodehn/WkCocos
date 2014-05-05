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
WkSrcDir ( "Classes" )
WkIncludeDir ( "Include" )
WkDataDir ( "Resources")
WkTestDir ( "Tests" )
WkDocDir ( "Docs" )

#Build rules
include("depends/cocos2d-wk.cmake")

#WkDepends("Custom Depend/ProjectEsub_src" REQUIRED)

WkBuild( LIBRARY )

#Linking with all libraries
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBRARIES})

if (MSVC)
	WkAddCFlags(Debug "-DCOCOS2D_DEBUG=1")
	WkAddCXXFlags(Debug "-DCOCOS2D_DEBUG=1")

	WkAddCFlags(All "-D_CRT_SECURE_NO_WARNINGS")
	WkAddCXXFlags(All "-D_CRT_SECURE_NO_WARNINGS")

elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    WkAddCFlags(Debug "-DCOCOS2D_DEBUG=1")
    WkAddCXXFlags(Debug "-DCOCOS2D_DEBUG=1")

	WkAddCFlags(All "-std=c99")
	WkAddCXXFlags(All "-std=c++11")

endif()

#Testing
#WkTestBuild( maintestB )
#WkTestData( maintestB data.txt )
#WkTestRun( ProjectB maintestB )
