cmake_minimum_required(VERSION 3.00)
project(imgui)

find_package(GLEW CONFIG REQUIRED)

set(IMGUI_SRC
${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
${CMAKE_CURRENT_LIST_DIR}/imgui_demo.cpp 
${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp 
${CMAKE_CURRENT_LIST_DIR}/imgui_extensions.cpp 
${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.cpp 
${CMAKE_CURRENT_LIST_DIR}/imgui_impl_glfw.cpp 
${CMAKE_CURRENT_LIST_DIR}/imgui_stdlib.cpp 
${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
${CMAKE_CURRENT_LIST_DIR}/implot.cpp
${CMAKE_CURRENT_LIST_DIR}/implot_demo.cpp
${CMAKE_CURRENT_LIST_DIR}/ImGuiFileDialog.cpp
)

add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_link_libraries(imgui PUBLIC GLEW::GLEW)
export(TARGETS imgui FILE imguiTargets.cmake)