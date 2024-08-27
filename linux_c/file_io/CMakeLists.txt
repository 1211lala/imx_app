cmake_minimum_required(VERSION 3.10)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB FILE ${PROJECT_SOURCE_DIR}/src/*.c)
add_executable(main ${FILE})