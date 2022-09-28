cmake_minimum_required(VERSION 3.10)

# set project name
project(openGL-FPSdemo VERSION 0.1)

configure_file(src/main.h.in src/main.h)

# add the executable
add_executable(openGL-FPSdemo src/main.cpp)