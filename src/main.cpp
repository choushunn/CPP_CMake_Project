#include <iostream>
#include "math.h"
#include "student.h"
#include <fmt/core.h>
#include <spdlog/spdlog.h>

int main() {
    fmt::print("===== C++ 多文件 CMake 项目（vcpkg fmt 已启用）=====\n");
    spdlog::info("spdlog 日志模块已启用");

    // 调用 math 模块
    int a = 10, b = 3;
    fmt::print("{} + {} = {}\n", a, b, add(a, b));
    fmt::print("{} - {} = {}\n", a, b, sub(a, b));

    // 调用 student 模块
    print_info();

    std::cout << "程序运行成功！" << '\n';
    return 0;
}
