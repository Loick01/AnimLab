cmake_minimum_required(VERSION 3.10)

project(animlab VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    src/main.cpp
    src/window.cpp
    src/application.cpp
    src/event.cpp
    src/chain.cpp
)

set(SFML_LIBRARIES
    sfml-graphics 
    sfml-window
    sfml-system
)

file(GLOB IMGUI_SOURCES
    external/imgui/src/*.cpp
)

find_package(OpenGL REQUIRED)

add_executable(animlab ${SOURCES} ${OPENGL_INCLUDE_DIR} ${IMGUI_SOURCES})

target_include_directories(animlab PRIVATE 
    include 
    external/imgui/include
)
    
target_link_libraries(animlab PRIVATE ${OPENGL_gl_LIBRARY} ${SFML_LIBRARIES})