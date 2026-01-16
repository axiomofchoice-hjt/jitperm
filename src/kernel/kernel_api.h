#pragma once

#include "base/jit.h"

enum class ISA : uint8_t { Unknown, Scalar, SSEPack };

template <typename T>
using kernel_fn = void (*)(const T* input, T* output);

template <typename T>
using jit_fn = jit::unique_function<kernel_fn<T>>;

template <typename T, ISA isa>
jit_fn<T> gen_kernel(std::span<const size_t> permutation);
