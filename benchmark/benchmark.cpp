#include <chrono>
#include <print>
#include <span>
#include <vector>

#include "kernel/kernel_api.h"

template <class Tp>
inline __attribute((always_inline)) void DoNotOptimize(Tp&& value) {
#if defined(__clang__)
    asm volatile("" : "+r,m"(value) : : "memory");
#else
    asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

struct BenchmarkParams {
    constexpr static size_t kDefaultWarmup = 100000;
    constexpr static size_t kDefaultIterations = 100000;
    size_t warmup = kDefaultWarmup;
    size_t iterations = kDefaultIterations;
};

template <typename T>
void benchmark_kernel(
    void (*kernel)(const T*, T*), const T* input, std::vector<T>& output, BenchmarkParams params = {}) {
    using clock = std::chrono::high_resolution_clock;

    for (size_t i = 0; i < params.warmup; ++i) {
        kernel(input, output.data());
    }

    auto start = clock::now();
    for (size_t i = 0; i < params.iterations; ++i) {
        kernel(input, output.data());
    }
    auto end = clock::now();

    double ns_per_call =
        std::chrono::duration<double, std::nano>(end - start).count() / static_cast<double>(params.iterations);
    std::println("Average time per call: {} ns", ns_per_call);
}

int main() {
    constexpr int32_t input[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr size_t permutation[] = {3, 0, 1, 6, 8, 2, 5};

    std::vector<int32_t> output(std::size(permutation), -1);

    auto scalar_kernel = gen_kernel<int32_t, ISA::Scalar>(permutation);
    auto sse_kernel = gen_kernel<int32_t, ISA::SSEPack>(permutation);

    std::print("--- Scalar Kernel ---\n");
    benchmark_kernel(scalar_kernel.get(), input, output);

    std::print("--- SSEPack Kernel ---\n");
    benchmark_kernel(sse_kernel.get(), input, output);

    return 0;
}
