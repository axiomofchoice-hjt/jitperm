#pragma once

template <typename T>
using kernel_fn = void (*)(const T* input, T* output);
