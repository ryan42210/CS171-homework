cmake_minimum_required(VERSION 3.0.0)
project(nurbsSurface VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
#add opengl
if(APPLE OR WIN32)
find_package(OpenGL)
endif()
# add glfw
if(WIN32)
include_directories(3rdLibs/glfw-3.3.2/include)
link_directories(3rdLibs/lib)
include_directories(3rdLibs/GL)
else()
find_package(glfw3)
endif()
# add glad
include_directories(3rdLibs/glad/include)
add_library(glad 3rdLibs/glad/src/glad.c)
# find source codes
# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	src/*.c
	src/*.cc
	src/*.hpp
	src/*.h
    src/*.cpp)
if(WIN32)
add_definitions(-D_AMD64_)
endif()
message("Following files are complied " ${SOURCE_FILES})
add_executable(nurbsSurface ${SOURCE_FILES})
if(WIN32)
target_link_libraries(nurbsSurface glad glfw3 ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES})
else()
target_link_libraries(nurbsSurface glad glfw ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES})
endif()
