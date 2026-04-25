cmake_minimum_required(VERSION 3.16)
project(multi_file_demo CXX)
include(GNUInstallDirs)

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

# 强制接入 vcpkg：必须通过 vcpkg toolchain 提供 fmt 与 spdlog。
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(demo PRIVATE fmt::fmt spdlog::spdlog)

# 安装规则
install(TARGETS demo
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 同步第三方运行时依赖（如 vcpkg 的 fmt 动态库）到安装目录。
install(FILES
    $<TARGET_FILE:fmt::fmt>
    $<TARGET_FILE:spdlog::spdlog>
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 同步 MinGW 运行时依赖，避免运行时从 PATH 读取到不兼容 DLL。
install(FILES
    "C:/Programs/msys64/ucrt64/bin/libstdc++-6.dll"
    "C:/Programs/msys64/ucrt64/bin/libgcc_s_seh-1.dll"
    "C:/Programs/msys64/ucrt64/bin/libwinpthread-1.dll"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)