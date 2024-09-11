cmake_minimum_required(VERSION 3.0)
# 设置工程名称
project(MyProject)

# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

# 查找源文件并存储在 FILE 变量中
file(GLOB FILE ${PROJECT_SOURCE_DIR}/src/*.c)

# 设置编译器
# set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
# set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_C_STANDARD 99)
# 添加可执行文件
add_executable(main ${FILE})

# 包含头文件
include_directories(    ${PROJECT_SOURCE_DIR}/include
                        # 链接tslib编译生成头文件位于ubuntu主机
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/tslib_1.21/tslib/include
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/jpeg/include
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/zlib/include
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/libpng/include
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/freetype/include/freetype2
                        ) 

# 链接库
target_link_libraries(  main   
                        m   #数学库
                        pthread 
                        # 链接tslib编译生成的动态库位于ubuntu主机
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/tslib_1.21/tslib/lib/libts.so
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/jpeg/lib/libjpeg.so 
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/libpng/lib/libpng.so
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/zlib/lib/libz.so
                        /home/liuao/imx6ull/user_uboot_linux_rootfs/third_lib/freetype/lib/libfreetype.so
                        )


