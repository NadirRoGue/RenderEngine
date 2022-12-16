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

function(compile_spirv_shader SHADER_PATH)
    message(STATUS "SPIR-V compiling ${SHADER_PATH}")
    get_filename_component(SHADER_NAME ${SHADER_PATH} NAME_WE)
    get_filename_component(SHADER_DIR ${SHADER_PATH} DIRECTORY)
    execute_process(COMMAND ${VULKAN_PLATFORM_HOME}/bin/glslc ${SHADER_PATH} -o ${SHADER_DIR}/${SHADER_NAME}.spv
        RESULT_VARIABLE COMPILE_RESULT
        OUTPUT_QUIET)

    if(NOT ${COMPILE_RESULT} EQUAL 0)
        message(FATAL_ERROR "Could not compile ${SHADER_PATH} shader to SPIR-V")
    endif()
endfunction()
