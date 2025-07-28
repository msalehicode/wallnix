cmake_minimum_required(VERSION 3.16)

project(wallnix VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add QuickDialogs here
find_package(Qt6 6.2 COMPONENTS Quick Widgets QuickDialogs2 REQUIRED)
find_package(X11 REQUIRED)

qt_add_executable(appwallnix
    main.cpp
)

qt_add_qml_module(appwallnix
    URI wallnix
    VERSION 1.0
    QML_FILES main.qml
    SOURCES wallpapermanager.h wallpapermanager.cpp
)

set_target_properties(appwallnix PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_link_libraries(appwallnix
    PRIVATE Qt6::Quick Qt6::Widgets Qt6::QuickDialogs2
    X11::X11           # Add X11 linkage
)

include(GNUInstallDirs)
install(TARGETS appwallnix
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
