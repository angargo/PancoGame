cmake_minimum_required (VERSION 2.6)
project(Serda)

# By default, enable debug
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set C++ 11
set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_NAME "Serda")
file(GLOB SOURCES "*.cpp")
add_executable(${EXECUTABLE_NAME} ${SOURCES})


# Find and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
# Find at least version 2.0
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()



