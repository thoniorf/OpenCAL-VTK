cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(OpenCAL 1.1 COMPONENTS  OpenCAL  REQUIRED)
find_package(GLUT REQUIRED)

if(NOT MSVC)
  find_library(M_LIB m)
else()
  # not needed on windows
  set(M_LIB "")
endif()

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${OPENCAL_INCLUDE_DIR}
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

file(GLOB SRC
    "include/*.h"
    "source/*.cpp"
    "source/*.c"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(cal_heat ${SRC})
target_link_libraries(cal_heat ${OPENCAL_LIBRARIES} ${M_LIB} )
