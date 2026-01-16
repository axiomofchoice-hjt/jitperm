#include <algorithm>

#include "interface.h"

template <>
jit_fn<int32_t> gen_kernel<int32_t, ISA::SSE>(
    std::span<const size_t> permutation) {
    size_t source_size = *std::ranges::max_element(permutation) + 1;
    size_t target_size = permutation.size();
    return jit::gen<kernel_fn<int32_t>>([=](asmjit::x86::Assembler& as) {
        namespace x86 = asmjit::x86;

        as.ret();
    });
}
