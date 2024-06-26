cmake_minimum_required(VERSION 3.16)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/.cmake/)
include(GetGitRevisionDescription)

project(container-desktop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/debug)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/release)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick WebSockets)

qt_standard_project_setup(REQUIRES 6.5)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/ViewModel/*.cpp
    ${CMAKE_SOURCE_DIR}/src/Infrastructure/Cache/*.cpp
    ${CMAKE_SOURCE_DIR}/src/Infrastructure/Network/*.cpp
    ${CMAKE_SOURCE_DIR}/src/Infrastructure/Utility/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

file(GLOB_RECURSE SOURCE_IMPORT_PATHS
    ${CMAKE_SOURCE_DIR}/src/ViewModel/*.h
)

foreach(filepath ${SOURCE_IMPORT_PATHS})
    string(REPLACE "${CMAKE_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND SOURCE_IMPORT_FILES ${filename})
endforeach(filepath)

set(VERSION_RC_PATH "")
set(WIN32_MANIFEST_PATH "")

if(WIN32)
    set(VERSION_RC_PATH ${CMAKE_SOURCE_DIR}/container-desktop.rc)
    configure_file(
        ${CMAKE_SOURCE_DIR}/.cmake/version_exe.rc.in
        ${VERSION_RC_PATH}
    )
    configure_file(
        ${CMAKE_SOURCE_DIR}/.cmake/InstallerScript.iss.in
        ${CMAKE_SOURCE_DIR}/action-cli/InstallerScript.iss
    )
    set(WIN32_MANIFEST_PATH ${CMAKE_SOURCE_DIR}/app.manifest)
endif()

if(APPLE)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER ${IDENTIFIER})
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT})

    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${spectral_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${spectral_VERSION})

    set(ICON_NAME "app.icns")
    set(${PROJECT_NAME}_MAC_ICON "${CMAKE_SOURCE_DIR}/${ICON_NAME}")
    set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME})
    set_property(SOURCE "${${PROJECT_NAME}_MAC_ICON}" PROPERTY
        MACOSX_PACKAGE_LOCATION Resources)
endif()

qt_add_executable(container-desktop
    ${${PROJECT_NAME}_MAC_ICON}
    ${SRC_FILES}
    ${VERSION_RC_PATH}
    ${WIN32_MANIFEST_PATH}
)

qt_add_qml_module(container-desktop
    URI container_desktop
    VERSION 1.0
    SOURCES ${SOURCE_IMPORT_FILES}
    RESOURCES res.qrc
    RESOURCES qml.qrc
)

target_include_directories(container-desktop
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/ViewModel
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(container-desktop PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/.cmake/Info.plist.in"
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(container-desktop
    PRIVATE Qt6::Quick
    Qt6::WebSockets
)

include(GNUInstallDirs)
install(TARGETS container-desktop
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
