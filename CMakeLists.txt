set(PROJECT_NAME lxqt-connman-applet)
cmake_minimum_required( VERSION 3.0 )
project(${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

add_subdirectory(applet)

