cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project(lxqt-connman-applet)

# Minimum Versions
set(LXQT_MINIMUM_VERSION "0.14.0")
set(QT_MINIMUM_VERSION "5.7.1")
set(QTXDG_MINIMUM_VERSION "3.3.0")

find_package(Qt5Widgets ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5DBus ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Svg ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5LinguistTools ${QT_MINIMUM_VERSION} REQUIRED)

find_package(lxqt ${LXQT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Xdg ${QTXDG_MINIMUM_VERSION} REQUIRED)

include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)
include(LXQtTranslate)

file(GLOB SRCS *.cpp clickableframe.h *.xml *.qrc *ui)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(iconviewer)

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
