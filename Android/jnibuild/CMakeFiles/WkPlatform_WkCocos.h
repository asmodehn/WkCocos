/* *
 *  Copyright (c) 2009, Asmodehn's Corp.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *      - Redistributions of source code must retain the above copyright notice,
 * 	    this list of conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright
 * 		notice, this list of conditions and the following disclaimer in the
 * 	    documentation and/or other materials provided with the distribution.
 *      - either the name of the Asmodehn's Corp. nor the names of its
 * 	    contributors may be used to endorse or promote products derived
 * 	    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *  THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef WKPLATFORM_WkCocos_HH
#define WKPLATFORM_WkCocos_HH

/* Project detection */
#define WK_WkCocos_PROJECT_NAME "WkCocos"

/* Platform type detection, CMake Based */

#define WK_WkCocos_PLATFORM "UNIX"

/* #undef WK_WkCocos_PLATFORM_IS_WIN32 */
/* #undef WK_WkCocos_PLATFORM_IS_APPLE */
#define WK_WkCocos_PLATFORM_IS_UNIX

/* Advanced System detection, CMake based */

#define WK_WkCocos_SYSTEM_NAME "Linux"
#define WK_WkCocos_SYSTEM_VERSION "1"
#define WK_WkCocos_SYSTEM_PROCESSOR "armv7-a"

/* #undef WK_WkCocos_SYSTEM_IS_WINDOWS */
/* #undef WK_WkCocos_SYSTEM_IS_MACOSX */
/* #undef WK_WkCocos_SYSTEM_IS_LINUX */

/* Compiler type detection, CMake based */

#define WK_WkCocos_COMPILER "Unknown"

/* #undef WK_WkCocos_COMPILER_IS_MSVC */
#define WK_WkCocos_COMPILER_IS_GNUCC
#define WK_WkCocos_COMPILER_IS_GNUCXX

/* Advanced Compiler Environment Detection */
#define WK_WkCocos_HAS_UNISTD_H

#ifdef WK_WkCocos_HAS_UNISTD_H
 #define WK_WkCocos_PLATFORM_POSIX
#endif

/*adjusting declspec values*/

/* Generic helper definitions for shared library support */
#if defined(WK_WkCocos_PLATFORM_IS_WIN32)
#define WK_WkCocos_HELPER_DLL_IMPORT __declspec(dllimport)
#define WK_WkCocos_HELPER_DLL_EXPORT __declspec(dllexport)
#define WK_WkCocos_HELPER_DLL_LOCAL
#else
#if defined(WK_WkCocos_COMPILER_IS_GNUCC) && (__GNUC__ >= 4)
#define WK_WkCocos_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
#define WK_WkCocos_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
#define WK_WkCocos_HELPER_DLL_LOCAL  __attribute__ ((visibility("hidden")))
#else
#define WK_WkCocos_HELPER_DLL_IMPORT
#define WK_WkCocos_HELPER_DLL_EXPORT
#define WK_WkCocos_HELPER_DLL_LOCAL
#endif
#endif



/*
 Now we use the generic helper definitions above to define WK_API and WK_LOCAL.
 WK_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
 WK_LOCAL is used for non-api symbols.
*/


#define WK_WkCocos_WkCocos_API
#define WK_WkCocos_WkCocos_LOCAL


#ifndef NULL
#define NULL 0
#endif

/* Dependency detection, Important to propagate build configuration of libraries... */
/* #undef WK_WkCocos_FOUND_STLPORT */
#ifdef WK_WkCocos_FOUND_STLPORT
    /* check user_config.h for the following settings */
    /* To work as expected, WkPlatform_WkCocos.h has to be included in all files, before including any stl header */
    /* if it is not possible for some reason, you should modify the stlport/stl/user_config.h header manually */

    /* This prevent STLport to rename std:: and therefore avoids confusion and linkage problem with 3d party libs */
    /* it also should allow to use multiple STL implementation in the same project */
    #define _STLP_DONT_REDEFINE_STD 1 
    #define _STLP_WHOLE_NATIVE_STD 1
    #ifdef _DEBUG
        #define _STLP_DEBUG 1
/* This STLP_DEBUG currently generates errors with imported vendor STL :
In file included from /usr/include/stlport/stl/_tree.h:657,
                 from /usr/include/stlport/stl/_map.h:34,
                 from /usr/include/stlport/map:36,
/usr/include/stlport/stl/_tree.c:320: error: expected initializer before '<' token
*/
    #endif
#endif

/* NB : This must be in sync with WkUtils.cmake macros and variables naming conventions... */
/* #undef WK_WkCocos_FOUND_SDLIMAGE */
/* #undef WK_WkCocos_FOUND_SDLMIXER */
/* #undef WK_WkCocos_FOUND_SDLTTF */
/* #undef WK_WkCocos_FOUND_SDLNET */
/* #undef WK_WkCocos_FOUND_OPENGL */
/* #undef WK_WkCocos_FOUND_GUICHAN */
/* #undef WK_WkCocos_FOUND_MySql */
/* #undef WK_WkCocos_FOUND_MySQLpp */
/* #undef WK_WkCocos_FOUND_Bullet */
/* #undef WK_WkCocos_FOUND_LuaJIT */


/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C"
{
#endif


/* Basic C function to display Wk Configuration on runtime */
WK_WkCocos_WkCocos_LOCAL int WkDebug();


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif



#endif /* WKPLATFORM_WkCocos_HH */
