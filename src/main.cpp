#include <asmjit/asmjit.h>

#include <cstdint>
#include <print>
#include <vector>

#include "assert.h"
#include "interface.h"

int main() try {
    std::vector<int32_t> in = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<size_t> permutation = {3, 0, 1, 6, 8, 2, 5};
    {
        std::vector<int32_t> out(permutation.size(), -1);
        auto fn = gen_kernel<int32_t, ISA::Scalar>(permutation);
        fn.get()(in.data(), out.data());
        std::println("{}", out);
    }
    {
        std::vector<int32_t> out(permutation.size(), -1);
        auto fn = gen_kernel<int32_t, ISA::SSEStore>(permutation);
        fn.get()(in.data(), out.data());
        std::println("{}", out);
    }
} catch (const assertion_error& e) {
    std::println("Assertion error: {}", e.what());
    return 1;
}
