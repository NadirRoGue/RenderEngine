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

include(FetchContent)

# =============================================================================
# GLM
# =============================================================================
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glm")
    message(STATUS "Downloading GLM from submodules")
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
        GIT_SHALLOW ON
        GIT_SUBMODULES_RECURSE OFF
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm
    )
    FetchContent_Populate(glm)
endif()

add_library(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/glm>
    $<INSTALL_INTERFACE:include>)

# =============================================================================
# imgui
# =============================================================================
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
    message(STATUS "Downloading imgui from submodules")
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.89.2
        GIT_SHALLOW ON
        GIT_SUBMODULES_RECURSE OFF
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    )
    FetchContent_Populate(imgui)
endif()

add_library(imgui INTERFACE)
target_include_directories(imgui SYSTEM INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/imgui>
    $<INSTALL_INTERFACE:include>)

# =============================================================================
# imgui
# =============================================================================
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glfw")
    message(STATUS "Downloading glfw from submodules")
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.8
        GIT_SHALLOW ON
        GIT_SUBMODULES_RECURSE OFF
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw
    )
    FetchContent_Populate(glfw)
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "")
add_subdirectory(glfw)
