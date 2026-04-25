cmake_minimum_required(VERSION 3.16)
project(multi_file_demo CXX)

# C++ 语言标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 统一使用 UTF-8 字符集，保证中文字符串在编译和运行时一致
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# 头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 自动收集所有 .cpp 文件
file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# 生成可执行文件
add_executable(demo ${SRC_FILES})