#include "math.h"

#include <cstdlib>
#include <iostream>

namespace {
bool expect_eq(int actual, int expected, const char* case_name) {
    if (actual != expected) {
        std::cerr << "[FAIL] " << case_name << ": expected " << expected
                  << ", got " << actual << '\n';
        return false;
    }
    std::cout << "[PASS] " << case_name << '\n';
    return true;
}
}  // namespace

int main() {
    bool ok = true;
    ok &= expect_eq(add(1, 2), 3, "add positive numbers");
    ok &= expect_eq(add(-1, 1), 0, "add with opposite signs");
    ok &= expect_eq(sub(5, 3), 2, "sub positive numbers");
    ok &= expect_eq(sub(3, 5), -2, "sub resulting in negative");

    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
