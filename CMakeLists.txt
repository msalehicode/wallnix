cmake_minimum_required(VERSION 3.16)

project(wallnix VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add QuickDialogs here
find_package(Qt6 6.2 COMPONENTS Quick Widgets QuickDialogs2 OpenGLWidgets REQUIRED)
find_package(X11 REQUIRED)

qt_add_executable(appwallnix
    main.cpp
)

qt_add_qml_module(appwallnix
    URI wallnix
    VERSION 1.0
    QML_FILES main.qml
    SOURCES wallpapermanager.h
    # SOURCES wallpapermanger_particle_example.h
    # SOURCES wallpapermanager_shapedrawAndmove_example.h
    # SOURCES wallpapermanager_shader_example.h
    QML_FILES changeWallpaper.qml
    QML_FILES settings.qml
    QML_FILES CustomDialogMessage.qml
    QML_FILES CustomButton.qml
    QML_FILES CustomSingleButton.qml
    QML_FILES homePage.qml
    RESOURCES resources/tray.svg
    RESOURCES resources/wallpaper.gif
)



set_target_properties(appwallnix PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_link_libraries(appwallnix
    PRIVATE Qt6::Quick Qt6::Widgets Qt6::QuickDialogs2 Qt6::OpenGLWidgets
    X11::X11           # Add X11 linkage
)

include(GNUInstallDirs)
install(TARGETS appwallnix
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)



# copy resourses beside output
set(RESOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/wallpaper.gif"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/tray.svg"
)
set(DEST_DIR "${CMAKE_BINARY_DIR}/resources")
file(MAKE_DIRECTORY ${DEST_DIR})
add_custom_command(
    TARGET appwallnix POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${RESOURCE_FILES}
            ${DEST_DIR}
    COMMENT "Copying resources to build folder"
)

