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

configure_file(engine/Version.h.in ${CMAKE_BINARY_DIR}/include/engine/Version.h)

file(GLOB_RECURSE ENGINE_HEADERS RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.h)
file(GLOB_RECURSE ENGINE_SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)

if(WIN32)
    list(REMOVE_ITEM ENGINE_HEADERS "engine/detail/X11Window.h")
    list(REMOVE_ITEM ENGINE_SOURCES "engine/detail/X11Window.cpp")
elseif(UNIX)
    list(REMOVE_ITEM ENGINE_HEADERS "engine/detail/Win32Window.h")
    list(REMOVE_ITEM ENGINE_SOURCES "engine/detail/Win32Window.cpp")
endif()

add_library(RenderEngine STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(RenderEngine PUBLIC Vulkan)
target_link_libraries(RenderEngine PRIVATE ${WINDOW_SYSTEM_LIBS} ${CMAKE_DL_LIBS})
target_include_directories(RenderEngine PRIVATE SYSTEM ${WINDOW_SYSTEM_INCLUDES})

set(BUILD_INCUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_BINARY_DIR}/include)
target_include_directories(RenderEngine PUBLIC
    "$<BUILD_INTERFACE:${BUILD_INCUDE_DIRS}>"
    "$<INSTALL_INTERFACE:include>")
