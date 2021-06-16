cmake_minimum_required(VERSION 3.16)
project(cpp_tests)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

set(IMGUI dep/imgui/)
add_executable(cpp_tests opengl.cpp Renderer.cpp VertexBuffer.cpp IndexBuffer.cpp VertexArray.cpp Shader.cpp Texture.cpp ${IMGUI}imgui.cpp ${IMGUI}backends/imgui_impl_glfw.cpp ${IMGUI}backends/imgui_impl_opengl3.cpp ${IMGUI}imgui_demo.cpp ${IMGUI}imgui_draw.cpp ${IMGUI}imgui_widgets.cpp ${IMGUI}imgui_widgets.cpp ${IMGUI}imgui_tables.cpp tests/TestClearColor.cpp tests/Test.cpp)

include_directories(${IMGUI})
if (WIN32)
    target_link_libraries(cpp_tests glew32 glfw3 opengl32 imm32)
else()
    target_link_libraries(cpp_tests GLEW glfw GL)
endif()