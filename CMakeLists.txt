cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Gazebo
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# Build our plugin
add_library(Led_plugin SHARED LedPlugin.cc)
set_property(TARGET Led_plugin PROPERTY CXX_STANDARD 17)
target_link_libraries(Led_plugin ${GAZEBO_libraries})
