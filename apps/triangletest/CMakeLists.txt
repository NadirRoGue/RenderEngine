set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/triangletest)

add_executable(TriangleTest TriangleTest.cpp)
target_link_libraries(TriangleTest PRIVATE Vulkan glfw glm)

add_executable(ModelTest ModelTest.cpp)
target_link_libraries(ModelTest PRIVATE Vulkan glfw glm)
target_include_directories(ModelTest PRIVATE ${CMAKE_CURRENT_LIST_DIR})

set(SHADERS_FOLDER ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/frag.frag DESTINATION ${SHADERS_FOLDER})
compile_spirv_shader(${SHADERS_FOLDER}/frag.frag ${SHADERS_FOLDER})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/vert.vert DESTINATION ${SHADERS_FOLDER})
compile_spirv_shader(${SHADERS_FOLDER}/vert.vert ${SHADERS_FOLDER})
