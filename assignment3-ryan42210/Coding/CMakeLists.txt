cmake_minimum_required(VERSION 3.0.0)
project(rayTracing VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "-w -O3")

set(NO_OMP 0)
# Flags for compiling
if(NO_OMP STREQUAL 1) 
    message(STATUS "OpenMP disabled.")
	add_definitions(-DNO_OMP)
else()
	message(STATUS "OpenMP enabled.")
	remove_definitions(-DNO_OMP)
endif()
	
# remove_definitions(-DNO_OMP)

# Find OpenMP
find_package(OpenMP)
include_directories(${OpenMP_CXX_INCLUDE_DIRS})

if(NO_OMP STREQUAL 0 AND OpenMP_FOUND)
	message(STATUS "OpenMP Found.")
	endif()
	
	# Add include path
	include_directories(
		src
		3rdLibs
		include
		)
		
# Find source files
file(GLOB_RECURSE SOURCE_FILES 
src/*.cpp
include/*.hpp)


# Specific flags
if(WIN32)
add_definitions(-D_AMD64_)
endif()

# Executables
add_executable(rayTracing ${SOURCE_FILES})
# Link OpenMP
if(NO_OMP STREQUAL 0)
	message(STATUS "OpenMP Linked.")
	target_link_libraries(rayTracing OpenMP::OpenMP_CXX)
endif()

