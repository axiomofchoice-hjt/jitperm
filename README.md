# jitperm - JIT Permutation Engine

`jitperm` 是一个 **运行时（JIT）生成 SIMD permutation / shuffle kernel** 的实验性项目。

在给定目标 permutation 的情况下，**搜索**一条由现有 SIMD 指令组成的最短或最低成本指令序列，并在运行时**即时生成并执行**对应的 kernel。

## 核心思想

算法：A* / beam search

最终指令：pshufb / punpck / palignr

代码生成：asmjit

## 构建

项目使用 **xmake**，要求支持 C++23 的编译器（clang / gcc）。

```bash
xmake config -m release
xmake
```
