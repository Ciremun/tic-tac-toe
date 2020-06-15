cmake_minimum_required(VERSION 3.16)
project(cpp_tests)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_tests opengl.cpp Renderer.cpp Renderer.h VertexBuffer.cpp VertexBuffer.h IndexBuffer.cpp
        IndexBuffer.h VertexArray.cpp VertexArray.h VertexBufferLayout.h Shader.cpp Shader.h dep/stb_image/stb_image.h
        dep/stb_image/stb_image.cpp Texture.cpp Texture.h dep/imgui/imgui.cpp dep/imgui/imgui.h
        dep/imgui/imgui_impl_glfw.cpp dep/imgui/imgui_impl_glfw.h dep/imgui/imgui_impl_opengl3.cpp
        dep/imgui/imgui_impl_opengl3.h dep/imgui/imconfig.h dep/imgui/imgui_demo.cpp dep/imgui/imgui_draw.cpp
        dep/imgui/imgui_internal.h dep/imgui/imgui_widgets.cpp tests/Test.h tests/TestClearColor.h tests/TestClearColor.cpp tests/Test.cpp)

include_directories(${CMAKE_SOURCE_DIR}/dep/GLEW/include ${CMAKE_SOURCE_DIR}/dep/GLFW/include
        ${CMAKE_SOURCE_DIR}/dep/stb_image ${CMAKE_SOURCE_DIR}/dep/glm ${CMAKE_SOURCE_DIR}/dep/imgui
        ${CMAKE_SOURCE_DIR})

add_library(libs opengl.cpp)

target_link_libraries(libs glew32 glfw3 opengl32)

target_link_libraries(libs -L${CMAKE_SOURCE_DIR}/dep/GLFW/lib -L${CMAKE_SOURCE_DIR}/dep/GLEW/lib/Release/x64)

target_link_libraries(cpp_tests libs)