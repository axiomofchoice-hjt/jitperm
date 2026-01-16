#include <asmjit/asmjit.h>

#include <cstdint>
#include <print>
#include <vector>

#include "assert.h"
#include "interface.h"

int main() try {
    std::vector<int32_t> in = {0, 1, 2, 3};
    std::vector<size_t> permutation = {3, 0, 1, 2};
    {
        std::vector<int32_t> out(4, -1);
        auto fn = gen_kernel<int32_t, ISA::Scalar>(permutation);
        fn.get()(in.data(), out.data());
        std::println("{}", out);
    }
    {
        std::vector<int32_t> out(4, -1);
        auto fn = gen_kernel<int32_t, ISA::SSE>(permutation);
        fn.get()(in.data(), out.data());
        std::println("{}", out);
    }
} catch (const assertion_error& e) {
    std::println("Assertion error: {}", e.what());
    return 1;
}
