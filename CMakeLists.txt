set(PROJECT_NAME lxqt-connman-applet)
cmake_minimum_required( VERSION 2.8.11 )
project(${PROJECT_NAME})

find_package(lxqt-qt5 REQUIRED QUIET)
find_package(Qt5Xdg REQUIRED QUIET)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5LinguistTools REQUIRED QUIET)

set(QTX_LIBRARIES Qt5::Widgets Qt5::DBus Qt5::Svg)

include(${LXQT_USE_FILE})
include(${QTXDG_USE_FILE})

include(LXQtTranslate)

if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Debug )
endif (NOT CMAKE_BUILD_TYPE)


set(HDRS
    dbus_types.h
)


set(SRCS
    main.cpp
    systemtray.cpp
    manager.cpp
    technology.cpp
    service.cpp
    agent.cpp
    dialog.cpp
    iconproducer.cpp
    dialog.ui
    net.connman.Agent.xml
    net.connman.Manager.xml
    net.connman.Service.xml
    net.connman.Technology.xml
)

set(MOCS
    systemtray.h
    manager.h
    service.h
    technology.h
    agent.h
    dialog.h
    iconproducer.h
)


set(UIS
    dialog.ui
)

set(QRCS
   resources.qrc
)


find_package(Qt5Widgets REQUIRED QUIET)
find_package(Qt5DBus REQUIRED QUIET)
find_package(Qt5Core REQUIRED QUIET)

qt5_wrap_cpp(MOCS_GENERATED ${MOCS})
qt5_wrap_ui(UIS_GENERATED ${UIS})
qt5_add_resources(QRCS_GENERATED ${QRCS})

qt5_add_dbus_adaptor(DBUS_GENERATED net.connman.Agent.xml agent.h Agent)

set_source_files_properties(net.connman.Manager.xml PROPERTIES INCLUDE dbus_types.h)
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Manager.xml net.connman.Manager)
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Service.xml net.connman.Service)
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Technology.xml net.connman.Technology)

message(INFO "DBUS_GENERATED: ${DBUS_GENERATED}")

include(${QTXDG_USE_FILE})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QTXDG_INCLUDE_DIRS}
)


set(APP_SHARE_DIR share/lxqt/${PROJECT_NAME})

#************************************************


# Translations **********************************lxqt-powermanagement/src/CMakeLists.txt:
lxqt_translate_desktop(DESKTOP_FILES SOURCES lxqt-connman-applet.desktop.in
)
#************************************************


add_executable(${PROJECT_NAME}
   ${SRCS}
   ${HDRS}
   ${QRCS_GENERATED}
   ${MOCS_GENERATED}
   ${DBUS_GENERATED}
   ${DESKTOP_FILES}
)

qt5_use_modules(lxqt-connman-applet Core Widgets DBus)


#add_dependencies(${PROJECT_NAME} ${LXQT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${QTX_LIBRARIES}  ${QTXDG_LIBRARIES} ${LXQT_LIBRARIES} )


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${CONFIG_FILES}    DESTINATION ${APP_SHARE_DIR})
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)

