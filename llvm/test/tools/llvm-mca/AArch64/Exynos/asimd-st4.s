# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=aarch64-linux-gnu -mcpu=exynos-m3 -resource-pressure=false -noalias=false < %s | FileCheck %s -check-prefixes=ALL,M3
# RUN: llvm-mca -mtriple=aarch64-linux-gnu -mcpu=exynos-m4 -resource-pressure=false -noalias=false < %s | FileCheck %s -check-prefixes=ALL,M4
# RUN: llvm-mca -mtriple=aarch64-linux-gnu -mcpu=exynos-m5 -resource-pressure=false -noalias=false < %s | FileCheck %s -check-prefixes=ALL,M5

st4	{v0.s, v1.s, v2.s, v3.s}[0], [sp]
st4	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp]

st4	{v0.d, v1.d, v2.d, v3.d}[0], [sp]
st4	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp]

st4	{v0.s, v1.s, v2.s, v3.s}[0], [sp], #16
st4	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp], #32

st4	{v0.d, v1.d, v2.d, v3.d}[0], [sp], #32
st4	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp], #64

st4	{v0.s, v1.s, v2.s, v3.s}[0], [sp], x0
st4	{v0.2s, v1.2s, v2.2s, v3.2s}, [sp], x0

st4	{v0.d, v1.d, v2.d, v3.d}[0], [sp], x0
st4	{v0.2d, v1.2d, v2.2d, v3.2d}, [sp], x0

# ALL:      Iterations:        100
# ALL-NEXT: Instructions:      1200

# M3-NEXT:  Total Cycles:      18603
# M3-NEXT:  Total uOps:        9000

# M4-NEXT:  Total Cycles:      4803
# M4-NEXT:  Total uOps:        4700

# M5-NEXT:  Total Cycles:      4803
# M5-NEXT:  Total uOps:        4700

# ALL:      Dispatch Width:    6

# M3-NEXT:  uOps Per Cycle:    0.48
# M3-NEXT:  IPC:               0.06
# M3-NEXT:  Block RThroughput: 76.5

# M4-NEXT:  uOps Per Cycle:    0.98
# M4-NEXT:  IPC:               0.25
# M4-NEXT:  Block RThroughput: 24.0

# M5-NEXT:  uOps Per Cycle:    0.98
# M5-NEXT:  IPC:               0.25
# M5-NEXT:  Block RThroughput: 24.0

# ALL:      Instruction Info:
# ALL-NEXT: [1]: #uOps
# ALL-NEXT: [2]: Latency
# ALL-NEXT: [3]: RThroughput
# ALL-NEXT: [4]: MayLoad
# ALL-NEXT: [5]: MayStore
# ALL-NEXT: [6]: HasSideEffects (U)

# ALL:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:

# M3-NEXT:   7      15    6.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp]
# M3-NEXT:   7      15    6.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M3-NEXT:   7      15    6.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp]
# M3-NEXT:   9      17    7.50           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M3-NEXT:   7      15    6.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], #16
# M3-NEXT:   7      15    6.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #32
# M3-NEXT:   7      15    6.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], #32
# M3-NEXT:   9      17    7.50           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #64
# M3-NEXT:   7      15    6.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], x0
# M3-NEXT:   7      15    6.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M3-NEXT:   7      15    6.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], x0
# M3-NEXT:   9      17    7.50           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0

# M4-NEXT:   2      2     1.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp]
# M4-NEXT:   4      4     2.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M4-NEXT:   2      2     1.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp]
# M4-NEXT:   5      8     4.00           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M4-NEXT:   3      2     1.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], #16
# M4-NEXT:   5      4     2.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #32
# M4-NEXT:   3      2     1.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], #32
# M4-NEXT:   6      8     4.00           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #64
# M4-NEXT:   3      2     1.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], x0
# M4-NEXT:   5      4     2.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M4-NEXT:   3      2     1.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], x0
# M4-NEXT:   6      8     4.00           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0

# M5-NEXT:   2      2     1.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp]
# M5-NEXT:   4      4     2.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp]
# M5-NEXT:   2      2     1.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp]
# M5-NEXT:   5      8     4.00           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp]
# M5-NEXT:   3      2     1.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], #16
# M5-NEXT:   5      4     2.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], #32
# M5-NEXT:   3      2     1.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], #32
# M5-NEXT:   6      8     4.00           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], #64
# M5-NEXT:   3      2     1.00           *            st4	{ v0.s, v1.s, v2.s, v3.s }[0], [sp], x0
# M5-NEXT:   5      4     2.00           *            st4	{ v0.2s, v1.2s, v2.2s, v3.2s }, [sp], x0
# M5-NEXT:   3      2     1.00           *            st4	{ v0.d, v1.d, v2.d, v3.d }[0], [sp], x0
# M5-NEXT:   6      8     4.00           *            st4	{ v0.2d, v1.2d, v2.2d, v3.2d }, [sp], x0
