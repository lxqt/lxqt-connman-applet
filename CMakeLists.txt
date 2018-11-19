cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project(lxqt-connman-applet)
add_subdirectory(iconviewer)

find_package(lxqt REQUIRED)
find_package(Qt5Xdg REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)
include(LXQtTranslate)

file(GLOB SRCS *.cpp clickableframe.h *.xml *.qrc *ui)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

qt5_add_resources(QRCS_GENERATED ${QRCS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QTXDG_INCLUDE_DIRS}
)
set(APP_SHARE_DIR share/lxqt/${PROJECT_NAME})

# Translations **********************************
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

target_link_libraries(${PROJECT_NAME}
                      lxqt
                      Qt5::Core
                      Qt5::Widgets
                      Qt5::DBus
                      Qt5::Svg)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${CONFIG_FILES}    DESTINATION ${APP_SHARE_DIR})
install(FILES   ${DESKTOP_FILES}   DESTINATION ${LXQT_ETC_XDG_DIR}/autostart)
