# PROJECT NAME
project(calvtk_siciddicaT_unsafe)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# COMPILER FLAG
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# find packages
find_package(OpenCAL 1.1 COMPONENTS  OpenCAL  REQUIRED)
find_package(VTK REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${OPENCAL_INCLUDE_DIR})
include(${VTK_USE_FILE})

file(GLOB SRC
    "include/*.h"
    "source/*.c"
)

add_executable(calvtk_sciddicaT-unsafe ${SRC})
target_link_libraries(calvtk_sciddicaT-unsafe ${OPENCAL_LIBRARIES} ${VTK_LIBRARIES})
