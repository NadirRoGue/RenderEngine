#
# The MIT License(MIT)
#
# Copyright © 2022 Nadir Roman Guerrero
#
# This file is part of RenderEngine <https://github.com/NadirRoGue/RenderEngine>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files(the “Software”), to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
#
# The above copyright notice and this permission notice shall be included in all copies or  substantial portions of
# the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# CMake configuration
set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RenderEngine VERSION 1.0.0)

if(APPLE)
    message(FATAL_ERROR "Apple is not supported")
endif()

if(UNIX)
    find_package(X11 REQUIRED)
    message(STATUS "Found X11 ${X11_LIBRARIES}")
    set(WINDOW_SYSTEM_LIBS ${X11_LIBRARIES})
    set(WINDOW_SYSTEM_INCLUDES ${X11_INCLUDE_DIR})
elseif(WIN32)
endif()

set(RENDERENGINE_COMPILE_OPTIONS -Wall -Wextra -pedantic-errors -Wconversion -Werror)

if(NOT VULKAN_PLATFORM_HOME)
    message(FATAL_ERROR "You must specify the path to your Vulkan installation as: -DVULKAN_PLATFORM_HOME=<path>")
endif()

add_library(Vulkan SHARED IMPORTED)
set_target_properties(Vulkan PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${VULKAN_PLATFORM_HOME}/include
    IMPORTED_LOCATION ${VULKAN_PLATFORM_HOME}/lib/libvulkan.so.1
)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CompileSPIRV)

add_subdirectory(deps)
add_subdirectory(src)
add_subdirectory(apps)
