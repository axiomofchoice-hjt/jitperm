#include <asmjit/asmjit.h>

#include <cstdint>
#include <print>

#include "assert.h"
#include "jit.h"

template <typename T>
using jit_kernel_fn = void (*)(const T* input, T* output);

namespace kernel {
// Emit machine code into the given assembler.
void emit_reverse_i32x4(asmjit::x86::Assembler& as) {
    namespace x86 = asmjit::x86;

    // xmm0 = *input
    as.movdqu(x86::xmm0, x86::ptr(x86::rdi));

    // reverse 4 x int32
    as.pshufd(x86::xmm0, x86::xmm0, 0x1B);

    // store result
    as.movdqu(x86::ptr(x86::rsi), x86::xmm0);

    as.ret();
}
}  // namespace kernel

int main() try {
    auto fn = jit::gen<jit_kernel_fn<int32_t>>(kernel::emit_reverse_i32x4);

    int32_t in[4] = {1, 2, 3, 4};
    int32_t out[4] = {};

    fn.get()(in, out);

    std::println("{}", out);
} catch (const assertion_error& e) {
    std::println("Assertion error: {}", e.what());
    return 1;
}
