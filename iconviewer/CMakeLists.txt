set(PROJECT_NAME icon-viewer)
project(${PROJECT_NAME})

file(GLOB SRCS *.cpp *ui ../iconproducer.cpp)

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

target_link_libraries(${PROJECT_NAME}
                      lxqt
                      Qt5::Core
                      Qt5::Widgets
                      Qt5::Svg)
