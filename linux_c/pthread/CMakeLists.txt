cmake_minimum_required(VERSION 3.0)
# 设置工程名称
project(MyProject)

# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 查找源文件并存储在 FILE 变量中
file(GLOB FILE ${PROJECT_SOURCE_DIR}/src/*.c)

# 设置编译器
# set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
# set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)


# 添加可执行文件
add_executable(main ${FILE})

# 链接 pthread 库
target_link_libraries(main pthread)
