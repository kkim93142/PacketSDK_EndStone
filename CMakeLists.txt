cmake_minimum_required(VERSION 3.15)

project(packet_sdk CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        endstone
        GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
        GIT_TAG v0.10.4
)
FetchContent_MakeAvailable(endstone)

file(GLOB_RECURSE SDK_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SDK/**/*.cpp"
)

endstone_add_plugin(${PROJECT_NAME}
        src/TestPlugin.cpp
        ${SDK_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
