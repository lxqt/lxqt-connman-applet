set(PROJECT_NAME icon-viewer)
cmake_minimum_required( VERSION 3.0.2 )
set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

project(${PROJECT_NAME})

find_package(lxqt REQUIRED QUIET)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)


if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug )
endif (NOT CMAKE_BUILD_TYPE)


file(GLOB SRCS *.cpp *ui ../iconproducer.cpp)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


qt5_add_resources(QRCS_GENERATED ${QRCS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QTXDG_INCLUDE_DIRS}
)


add_executable(${PROJECT_NAME}
   ${SRCS}
   ${QRCS_GENERATED}
)

qt5_use_modules(icon-viewer Core Widgets)


target_link_libraries(${PROJECT_NAME} 
                      lxqt
                      Qt5::Widgets 
                      Qt5::Svg)


