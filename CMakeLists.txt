# Declare the minimum required CMake version
cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)

# Name the project
project(Chess_Engine)
set(APPLICATION_LIBRARY ChessEngineLib)

# Request the required wxWidgets libs
# Turn off wxWidgets own precompiled header system, since
# it doesn't seem to work. The CMake version works much better.
set(wxBUILD_PRECOMP OFF)
find_package(wxWidgets COMPONENTS core base xrc html xml REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})

add_subdirectory(${APPLICATION_LIBRARY})
include_directories(${APPLICATION_LIBRARY})

include_directories(${miniaudio_SOURCE_DIR})



# Define a variable containing a list of source files for the project
set(SOURCE_FILES main.cpp ChessEngineApp.cpp ChessEngineApp.h pch.h ${MACOSX_BUNDLE_ICON_FILE})



# Define the build target for the executable
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCE_FILES})

# Link required libraries to the executable
target_link_libraries(${PROJECT_NAME} ${APPLICATION_LIBRARY})

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

if(APPLE)
    # When building for MacOS, also copy resources into the bundle resources
    set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources)
    file(COPY resources/ DESTINATION ${RESOURCE_DIR}/)
endif()