#pragma once

#include <asmjit/asmjit.h>

#include "assert.h"

namespace jit {
inline asmjit::JitRuntime& jit_runtime() {
    static asmjit::JitRuntime rt;
    return rt;
}

template <typename Fn>
struct jit_function {
    jit_function() = default;

    jit_function(asmjit::JitRuntime& rt, Fn fn) : rt_(&rt), fn_(fn) {}

    // 不允许拷贝（双重 release 会炸）
    jit_function(const jit_function&) = delete;
    jit_function& operator=(const jit_function&) = delete;

    // 允许移动
    jit_function(jit_function&& other) noexcept
        : rt_(other.rt_), fn_(other.fn_) {
        other.rt_ = nullptr;
        other.fn_ = nullptr;
    }

    jit_function& operator=(jit_function&& other) noexcept {
        if (this != &other) {
            reset();
            rt_ = other.rt_;
            fn_ = other.fn_;
            other.rt_ = nullptr;
            other.fn_ = nullptr;
        }
        return *this;
    }

    ~jit_function() { reset(); }

    Fn get() const { return fn_; }
    explicit operator bool() const { return fn_ != nullptr; }

    Fn operator*() const { return fn_; }

    void reset() {
        if (rt_ && fn_) {
            rt_->release(reinterpret_cast<void*>(fn_));
        }
        fn_ = nullptr;
        rt_ = nullptr;
    }

    asmjit::JitRuntime* rt_ = nullptr;
    Fn fn_ = nullptr;
};

template <typename Fn, typename Kernel>
jit_function<Fn> gen(const Kernel& kernel) {
    asmjit::CodeHolder code;
    code.init(jit_runtime().environment());

    asmjit::x86::Assembler as(&code);
    kernel(as);

    Fn fn = nullptr;
    assert_or_throw(jit_runtime().add(&fn, &code) == asmjit::kErrorOk);
    return jit_function<Fn>{jit_runtime(), fn};
}
}  // namespace jit
