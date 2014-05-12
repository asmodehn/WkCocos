# Install script for directory: /home/alexv/Projects/WkCocos

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/alexv/Projects/WkCocos/Android/arm-linux-androideabi-4.6/user")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/math/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/external/chipmunk/src/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/external/unzip/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/external/tinyxml2/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/audio/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/external/xxhash/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/base/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/2d/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/storage/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/ui/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/network/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/extensions/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/editor-support/spine/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/editor-support/cocosbuilder/cmake_install.cmake")
  include("/home/alexv/Projects/WkCocos/Android/jnibuild/depends/cocos2d-x/cocos/editor-support/cocostudio/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "/home/alexv/Projects/WkCocos/Android/jnibuild/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "/home/alexv/Projects/WkCocos/Android/jnibuild/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
