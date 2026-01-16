#include <asmjit/asmjit.h>

#include <algorithm>
#include <cstdint>
#include <print>

#include "assert.h"
#include "jit.h"

template <typename T>
using jit_fn = void (*)(const T* input, T* output);

jit::unique_function<jit_fn<int32_t>> gen_kernel_stub(
    std::span<const size_t> permutation) {
    size_t source_size = *std::ranges::max_element(permutation) + 1;
    size_t target_size = permutation.size();
    return jit::gen<jit_fn<int32_t>>([=](asmjit::x86::Assembler& as) {
        namespace x86 = asmjit::x86;

        for (size_t i = 0; i < target_size; ++i) {
            as.mov(x86::eax,
                   x86::dword_ptr(x86::rdi, permutation[i] * sizeof(int32_t)));
            as.mov(x86::dword_ptr(x86::rsi, i * sizeof(int32_t)), x86::eax);
        }

        as.ret();
    });
}

int main() try {
    auto fn = gen_kernel_stub(std::array<size_t, 4>{3, 0, 1, 2});

    int32_t in[4] = {0, 1, 2, 3};
    int32_t out[4] = {};

    fn.get()(in, out);

    std::println("{}", out);
} catch (const assertion_error& e) {
    std::println("Assertion error: {}", e.what());
    return 1;
}
