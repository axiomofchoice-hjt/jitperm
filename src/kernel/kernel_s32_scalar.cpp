#include <ranges>

#include "kernel_api.h"

template <>
jit_fn<int32_t> gen_kernel<int32_t, ISA::Scalar>(std::span<const size_t> permutation) {
    size_t target_size = permutation.size();
    return jit::gen<kernel_fn<int32_t>>([=](asmjit::x86::Assembler& as) {
        namespace x86 = asmjit::x86;
        auto input_ptr = x86::rdi;
        auto output_ptr = x86::rsi;

        for (size_t i : std::views::iota(size_t{0}, target_size)) {
            as.mov(x86::eax, x86::dword_ptr(input_ptr, permutation[i] * sizeof(int32_t)));
            as.mov(x86::dword_ptr(output_ptr, i * sizeof(int32_t)), x86::eax);
        }

        as.ret();
    });
}
