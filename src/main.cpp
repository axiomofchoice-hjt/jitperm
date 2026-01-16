#include <asmjit/asmjit.h>

#include <cstdint>
#include <print>
#include <vector>

#include "base/assert.h"
#include "kernel/kernel_api.h"

template <typename T>
void print(std::span<T> data) {
    std::print("[");
    for (size_t i = 0; i < data.size(); ++i) {
        std::print("{}", data[i]);
        if (i + 1 < data.size()) {
            std::print(", ");
        }
    }
    std::println("]");
}

int main() try {
    constexpr int32_t input[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr size_t permutation[] = {3, 0, 1, 6, 8, 2, 5};
    {
        std::vector<int32_t> output(std::size(permutation), -1);
        auto kernel = gen_kernel<int32_t, ISA::Scalar>(permutation);
        kernel.get()(input, output.data());
        print(std::span{output});
    }
    {
        std::vector<int32_t> output(std::size(permutation), -1);
        auto kernel = gen_kernel<int32_t, ISA::SSEPack>(permutation);
        kernel.get()(input, output.data());
        print(std::span{output});
    }
} catch (const assertion_error& e) {
    std::println("Assertion error: {}", e.what());
    return 1;
} catch (const std::exception& e) {
    std::println("Exception: {}", e.what());
    return 1;
} catch (...) {
    std::println("Unknown exception");
    return 1;
}
