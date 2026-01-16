#include <ranges>

#include "kernel_api.h"

template <>
jit_fn<int32_t> gen_kernel<int32_t, ISA::SSEPack>(std::span<const size_t> permutation) {
    size_t target_size = permutation.size();
    return jit::gen<kernel_fn<int32_t>>([=](asmjit::x86::Assembler& as) {
        namespace x86 = asmjit::x86;
        constexpr size_t vector_size = 4;

        for (size_t i : std::views::iota(size_t{0}, target_size / vector_size * vector_size) | std::views::stride(4)) {
            auto xmm = x86::xmm0;

            as.movd(xmm, x86::dword_ptr(x86::rdi, permutation[i] * sizeof(int32_t)));
            as.pinsrd(xmm, x86::dword_ptr(x86::rdi, permutation[i + 1] * sizeof(int32_t)), 1);
            as.pinsrd(xmm, x86::dword_ptr(x86::rdi, permutation[i + 2] * sizeof(int32_t)), 2);
            as.pinsrd(xmm, x86::dword_ptr(x86::rdi, permutation[i + 3] * sizeof(int32_t)), 3);
            as.movups(x86::xmmword_ptr(x86::rsi, i * sizeof(int32_t)), xmm);
        }

        for (size_t i : std::views::iota(target_size / vector_size * vector_size, target_size)) {
            as.mov(x86::eax, x86::dword_ptr(x86::rdi, permutation[i] * sizeof(int32_t)));
            as.mov(x86::dword_ptr(x86::rsi, i * sizeof(int32_t)), x86::eax);
        }

        as.ret();
    });
}
