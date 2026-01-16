#include <algorithm>
#include <ranges>

#include "interface.h"

template <>
jit_fn<int32_t> gen_kernel<int32_t, ISA::Scalar>(std::span<const size_t> permutation) {
    size_t source_size = *std::ranges::max_element(permutation) + 1;
    size_t target_size = permutation.size();
    return jit::gen<kernel_fn<int32_t>>([=](asmjit::x86::Assembler& as) {
        namespace x86 = asmjit::x86;

        for (size_t i : std::views::iota(size_t{0}, target_size)) {
            as.mov(x86::eax, x86::dword_ptr(x86::rdi, permutation[i] * sizeof(int32_t)));
            as.mov(x86::dword_ptr(x86::rsi, i * sizeof(int32_t)), x86::eax);
        }

        as.ret();
    });
}
