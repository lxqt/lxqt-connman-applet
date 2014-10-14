set(PROJECT_NAME lxqt-connman-applet)
cmake_minimum_required( VERSION 3.0 )
project(${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)


set(HDRS
)

set(SRCS
    main.cpp
    systemtray.cpp
)

set(MOCS
   systemtray.h
)


set(UIS
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
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Manager.xml net.connman.Manager)
qt5_add_dbus_interface(DBUS_GENERATED net.connman.Service.xml net.connman.Service)

message(INFO "DBUS_GENERATED: ${DBUS_GENERATED}")

#set(QTX_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5DBus_LIBRARIES} ${Qt5X11Extras_LIBRARIES})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
#    ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
#    ${LXQT_INCLUDE_DIR}
)


set(APP_SHARE_DIR share/lxqt/${PROJECT_NAME})

#************************************************


# Translations **********************************
#lxqt_translate_desktop(DESKTOP_FILES
#    SOURCES
#        resources/lxqt-connman-applet.desktop.in
#)
#************************************************


add_executable(${PROJECT_NAME}
   ${SRCS}
   ${QRCS_GENERATED}
   ${MOCS_GENERATED}
   ${DBUS_GENERATED}
)

qt5_use_modules(lxqt-connman-applet Core Widgets DBus)


#add_dependencies(${PROJECT_NAME} ${LXQT_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} ${QTX_LIBRARIES} ${LXQT_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES   ${CONFIG_FILES}    DESTINATION ${APP_SHARE_DIR})
install(FILES   ${DESKTOP_FILES}   DESTINATION share/applications)

