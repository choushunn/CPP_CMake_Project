cmake_minimum_required(VERSION 3.16)
project(multi_file_demo CXX)
include(GNUInstallDirs)
include(CTest)

# C++ 语言标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_WARNINGS "Enable strict compiler warnings." ON)

add_library(math STATIC src/math.cpp)
target_include_directories(math
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_library(student STATIC src/student.cpp)
target_include_directories(student
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(demo src/main.cpp)
target_link_libraries(demo PRIVATE math student)

# 强制接入 vcpkg：必须通过 vcpkg toolchain 提供 fmt 与 spdlog。
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(demo PRIVATE fmt::fmt spdlog::spdlog)

if(ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(math PRIVATE /W4 /utf-8)
        target_compile_options(student PRIVATE /W4 /utf-8)
        target_compile_options(demo PRIVATE /W4 /utf-8)
    else()
        target_compile_options(
            math
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -finput-charset=UTF-8
                -fexec-charset=UTF-8
        )
        target_compile_options(
            student
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -finput-charset=UTF-8
                -fexec-charset=UTF-8
        )
        target_compile_options(
            demo
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -finput-charset=UTF-8
                -fexec-charset=UTF-8
        )
    endif()
endif()

# 安装规则
install(TARGETS demo math student
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.21")
    install(TARGETS demo
        RUNTIME_DEPENDENCIES
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
            POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    )
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

if(BUILD_TESTING)
    add_executable(test_math tests/test_math.cpp)
    target_link_libraries(test_math PRIVATE math)
    target_include_directories(test_math PRIVATE ${PROJECT_SOURCE_DIR}/include)

    if(ENABLE_WARNINGS)
        if(MSVC)
            target_compile_options(test_math PRIVATE /W4 /utf-8)
        else()
            target_compile_options(
                test_math
                PRIVATE
                    -Wall
                    -Wextra
                    -Wpedantic
                    -finput-charset=UTF-8
                    -fexec-charset=UTF-8
            )
        endif()
    endif()

    add_test(NAME math_test COMMAND test_math)
endif()