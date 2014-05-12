#include "WkPlatform_WkCocos.h"

/* Basic C function to display Wk Configuration on runtime */
#include <stdio.h>

/*WK_WkCocos_LOCAL*/ int WkDebug()
{
    printf ( "\n%s has been built with WkCMake \n", WK_WkCocos_PROJECT_NAME );
    printf ( "Build Platform : %s\n", WK_WkCocos_PLATFORM );
    printf ( "Build System : %s %s %s\n", WK_WkCocos_SYSTEM_NAME, WK_WkCocos_SYSTEM_VERSION, WK_WkCocos_SYSTEM_PROCESSOR );
    printf ( "Compiler used : %s\n", WK_WkCocos_COMPILER );
    printf ( "The following external dependencies were detected :\n");
#ifdef WK_WkCocos_FOUND_SDLIMAGE
    printf ( "- SDL_image\n" ) ;
#endif
#ifdef WK_WkCocos_FOUND_SDLMIXER
    printf ( "- SDL_mixer\n" );
#endif
#ifdef WK_WkCocos_FOUND_SDLTTF
    printf ( "- SDL_ttf\n" );
#endif
#ifdef WK_WkCocos_FOUND_SDLNET
    printf ( "- SDL_net\n" );
#endif
#ifdef WK_WkCocos_FOUND_OPENGL
    printf ( "- OpenGL\n" );
#endif
#ifdef WK_WkCocos_FOUND_GUICHAN
    printf ( "- GUIchan\n");
#endif
#ifdef WK_WkCocos_FOUND_MySql
    printf ( "- MySql\n");
#endif
#ifdef WK_WkCocos_FOUND_MySQLpp
    printf ( "- MySQL++\n");
#endif
#ifdef WK_WkCocos_FOUND_Bullet
    printf ( "- Bullet\n");
#endif
#ifdef WK_WkCocos_FOUND_LuaJIT
    printf ( "- LuaJIT\n");
#endif

    return 0;
}


