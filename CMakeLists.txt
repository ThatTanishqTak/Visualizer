cmake_minimum_required(VERSION 3.27)

project(Visualizer LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings & MSVC-specific flags
if (MSVC)
    add_compile_options(/W4 /permissive- /EHsc)
endif()

# Source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_library(Visualizer SHARED ${SOURCES})

if(IS_MULTI_CONFIG)
  set(OUTPUT_CONFIG "$<CONFIG>")
else()
  set(OUTPUT_CONFIG "${CMAKE_BUILD_TYPE}")
endif()

set(OS_NAME ${CMAKE_SYSTEM_NAME})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH "x64")
else()
  set(ARCH "x86")
endif()

set(OUTPUT_DIR "${OS_NAME}-${OUTPUT_CONFIG}-${ARCH}/${PROJECT_NAME}")
set_target_properties(Visualizer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}
)

# Include paths for vendor libs (hooked later when we actually use them)
target_include_directories(Visualizer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/miniaudio
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/kissfft
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Catch2/include
)

# Windows plugin specifics
if (MSVC)
    target_compile_definitions(Visualizer PRIVATE
        UNICODE
        _UNICODE
    )
endif()

# Name the output exactly how Rainmeter will see the plugin
set_target_properties(Visualizer PROPERTIES
    OUTPUT_NAME "Visualizer"
)

# Optional: auto-copy the built DLL into Rainmeter's Plugins folder.
# Adjust this path if your Documents folder / Rainmeter path is different.
set(RAINMETER_PLUGIN_DIR
    "$ENV{USERPROFILE}/Documents/Rainmeter/Plugins"
    CACHE PATH "Rainmeter plugin output directory"
)

add_custom_command(
    TARGET Visualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RAINMETER_PLUGIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Visualizer>
            "${RAINMETER_PLUGIN_DIR}/$<TARGET_FILE_NAME:Visualizer>"
    COMMENT "Copying Visualizer.dll to Rainmeter plugins directory"
)