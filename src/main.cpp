#include <asmjit/asmjit.h>

#include <cstdint>
#include <print>

#include "assert.h"
#include "kernel_s32_scalar.h"

int main() try {
    auto fn = gen_kernel_scalar(std::array<size_t, 4>{3, 0, 1, 2});

    int32_t in[4] = {0, 1, 2, 3};
    int32_t out[4] = {};

    fn.get()(in, out);

    std::println("{}", out);
} catch (const assertion_error& e) {
    std::println("Assertion error: {}", e.what());
    return 1;
}
