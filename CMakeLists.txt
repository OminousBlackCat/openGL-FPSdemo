# project cmake version settings
cmake_minimum_required(VERSION 3.10)
# set project name
project(openGL-FPSdemo VERSION 0.1)

# add the executable
add_executable(openGL-FPSdemo 
    src/main.cpp
)

# set cpp standard with 11 version
set_property(TARGET openGL-FPSdemo PROPERTY CXX_STANDARD 11)
target_compile_options(openGL-FPSdemo PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)

target_link_libraries(openGL-FPSdemo
    PRIVATE glfw
    PRIVATE libglew_static
    PRIVATE glm
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/src/asset.hpp
)
target_include_directories(openGL-FPSdemo
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)