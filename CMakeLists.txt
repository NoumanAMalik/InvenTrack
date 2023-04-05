cmake_minimum_required(VERSION 3.25)
project(InvenTrack)

add_executable(InvenTrack InvenTrack.cpp)

set_target_properties(InvenTrack PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CMAKE_COLOR_MAKEFILE OFF
        )

add_library(scanlib STATIC scanin.cpp scanout.cpp)

target_link_libraries(InvenTrack PRIVATE scanlib)

# Create a custom target for testing InvenTrack
add_custom_target(test_InvenTrack
        COMMAND $<TARGET_FILE:InvenTrack> scanin
        COMMAND $<TARGET_FILE:InvenTrack> scanout
        DEPENDS InvenTrack
        )