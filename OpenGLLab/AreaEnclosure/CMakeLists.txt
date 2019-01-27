cmake_minimum_required(VERSION 2.8.11)

# Project name
project(ConvexHull)

set(CONFIGURATION_NAME "Debug")

# Set default locations
#set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/${CONFIGURATION_NAME})
set(CL_OUTPUT_DIRECTORY ../bin/${CONFIGURATION_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})


## Projects


# Top project
# Define some variables
set(PROJECT_CH_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(WORKSPACE_PATH "${CMAKE_CURRENT_LIST_DIR}")

#{{{{ User Code 1
# Place your code here
#}}}}

include_directories(
    .
    /usr/include
    /usr/local/include
    /usr/include/freetype2
    ${PROJECT_CH_PATH}/include
)

add_definitions(-O)
add_definitions(-std=c++14)
add_definitions(-Wall)
add_definitions(-g)
add_definitions(
    -D_LARGEFILE_SOURCE
    -D_LARGEFILE64_SOURCE
    -D_THREAD_SAFE
    -D_REENTRANT
)


## Linker options


if(WIN32)
    # Resource options
endif(WIN32)

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. -L/usr/lib -L/usr/local/lib ")

# Define the CXX sources
set ( CXX_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/ConvexHull.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GLShader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TestConvexHull.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -O -std=c++14 -Wall -fvisibility-inlines-hidden -g")

if(WIN32)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> ${RC_OPTIONS} -O coff -i <SOURCE> -o <OBJECT>")
endif(WIN32)

#{{{{ User Code 2
# Place your code here
#}}}}

add_executable(ConvexHull ${RC_SRCS} ${CXX_SRCS} ${C_SRCS})
target_link_libraries(ConvexHull ${LINK_OPTIONS})
add_library (ConvexHullLib SHARED ${CMAKE_CURRENT_LIST_DIR}/src/ConvexHull.cpp)

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
  target_include_directories(ConvexHull PRIVATE ${OPENGL_INCLUDE_DIR})
  target_link_libraries(ConvexHull PRIVATE ${OPENGL_gl_LIBRARY})
endif(OpenGL_FOUND)

target_link_libraries(ConvexHull PRIVATE
    GLEW
    glfw
    freetype
)

#{{{{ User Code 3
# Place your code here
#}}}}
