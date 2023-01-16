# release <-> debug: -DCMAKE_BUILD_TYPE=<Release, Debug>

cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 14)
project(moe)

#Download glm from github
include(FetchContent) 
FetchContent_Declare (glm                 
        GIT_REPOSITORY https://github.com/g-truc/glm.git
) 
FetchContent_MakeAvailable(glm)

#Locate GoogleTest
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

#Locate SDL
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(OpenGL_GL_PREFERENCE "GLVND")

#Locate GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

#boost
#find_package(Boost REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})

add_executable(moe main.cpp Window.cpp
                Shader.cpp
                App.cpp
                DrawableGL.cpp
                Camera2D.cpp)

target_link_libraries(moe
        ${SDL2_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES}
        glm
        pthread)