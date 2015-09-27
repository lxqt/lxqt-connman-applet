set(PROJECT_NAME lxqt-connman-applet)
cmake_minimum_required( VERSION 3.0.2 )
project(${PROJECT_NAME})

find_package(lxqt REQUIRED QUIET)
find_package(Qt5Xdg REQUIRED QUIET)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5LinguistTools REQUIRED QUIET)


if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug )
endif (NOT CMAKE_BUILD_TYPE)


file(GLOB SRCS *.cpp *.xml *.qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED QUIET)
find_package(Qt5DBus REQUIRED QUIET)

qt5_add_resources(QRCS_GENERATED ${QRCS})

qt5_add_dbus_adaptor(DBUS_GENERATED net.connman.Agent.xml agent.h Agent)

set_source_files_properties(net.connman.Manager.xml PROPERTIES INCLUDE dbus_types.h)
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Manager.xml net.connman.Manager)
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Service.xml net.connman.Service)
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Technology.xml net.connman.Technology)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QTXDG_INCLUDE_DIRS}
)


set(APP_SHARE_DIR share/lxqt/${PROJECT_NAME})


# Translations **********************************
include(LXQtTranslate)

file(GLOB TRANSLATE_SOURCES *.ui *.h *.cpp)

lxqt_translate_ts(QM_FILES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    SOURCES
         ${TRANSLATE_SOURCES}
    INSTALL_DIR
        "${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}"
)

lxqt_app_translation_loader(QM_LOADER ${PROJECT_NAME})

lxqt_translate_desktop(DESKTOP_FILES SOURCES lxqt-connman-applet.desktop.in)

#************************************************

add_executable(${PROJECT_NAME}
   ${SRCS}
   ${QRCS_GENERATED}
   ${DBUS_GENERATED}
   ${DESKTOP_FILES}
   ${QM_FILES}
   ${QM_LOADER}
)

qt5_use_modules(lxqt-connman-applet Core Widgets DBus)


target_link_libraries(${PROJECT_NAME} 
                      lxqt
                      Qt5::Widgets 
                      Qt5::DBus 
                      Qt5::Svg)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${CONFIG_FILES}    DESTINATION ${APP_SHARE_DIR})
install(FILES   ${DESKTOP_FILES}   DESTINATION ${LXQT_ETC_XDG_DIR}/autostart)

