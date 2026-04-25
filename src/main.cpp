#include <iostream>
#include "math.h"
#include "student.h"

int main() {
    std::cout << "===== C++ 多文件 CMake 项目 =====" << '\n';

    // 调用 math 模块
    int a = 10, b = 3;
    std::cout << a << " + " << b << " = " << add(a, b) << '\n';
    std::cout << a << " - " << b << " = " << sub(a, b) << '\n';

    // 调用 student 模块
    print_info();

    std::cout << "程序运行成功！" << '\n';
    return 0;
}
