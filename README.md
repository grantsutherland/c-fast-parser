# C Fast Parser 

A high performance CSV parser written in C

Bypasses the standard I/O bottlenecks of fopen and fread by utilizing POSIX system calls (mmap) to project files directly into virtual memory. It is designed for environments that need to ingest massive, high-dimensional datasets—like single-cell matrices or gigabytes of market tick data—faster than standard Python libraries allow.

## Core Features
* Zero-Copy Memory Mapping: Uses mmap to read files directly from disk to RAM.
* Lock-free Multithreading: Partitions data to be processed independently by threads, and then data is joined.
* Custom Pointer Arithmetic: Implements an optimized ASCII-to-float parser.
* Parallel Welford's Algorithm: Calculates running mean and variance in a single pass.
* Zero Dependencies: Requires no external libraries, in solely C.

## Architecture
* io_core: Handles file descriptors, fstat, and memory mapping.
* parser: The hot-loop that executes raw pointer traversal.
* compute: The mathematical backend.
* engine: Handles pthreads, memory boundaries, and stack allocation.

## Benchmarks

**Setup:** Apple M4 Pro (10 performance + 4 efficiency cores), 24 GB RAM, macOS 15.6,
compiled with `-O3 -march=native`. Two datasets: a **tall** file (100M rows x 3 numeric
columns, 2.38 GB) and a **wide** file (1M rows x 250 columns, 1.25 GB). All figures are
**warm cache** (data resident in RAM), reported as the **median of 5 runs**. Throughput is
`file_bytes / seconds`. The task computed for every tool is identical: mean + population
variance over every column.

> Numbers are in-memory parse throughput, not end-to-end from cold disk. On genuinely
> cold data the 7-8 GB/s figures would be capped by SSD read bandwidth.

### Tall file — 100M rows (2.38 GB)
| Tool | Time | GB/s | Speedup vs Pandas |
| :--- | :--- | :--- | :--- |
| Pandas (`read_csv` + mean + var) | 9.44 s | 0.25 | 1.0x |
| DuckDB | 0.83 s | 2.86 | 11.3x |
| Polars | 0.83 s | 2.88 | 11.4x |
| **C Fast Parser (8 threads)** | **0.33 s** | **7.31** | **29.0x** |

Versus the engines that actually parse-and-aggregate without materializing a DataFrame,
the C parser is **~2.6x faster than DuckDB** and **~2.5x faster than Polars** on this file.

### CPU core scaling (tall file)
Lock-free partitioning keeps efficiency high across the 10 performance cores. Efficiency is
`(T1 / Tn) / n`. The single-thread run is memory/fault-stalled (only 75% CPU), so 2 threads
are briefly super-linear.

| Threads | Time | GB/s | CPU Utilization | Scaling Efficiency |
| :---: | :--- | :--- | :--- | :---: |
| 1 | 2.38 s | 1.00 | 75% | 100% |
| 2 | 1.18 s | 2.02 | 198% | 101% |
| 4 | 0.63 s | 3.80 | 387% | 95% |
| 8 | 0.33 s | 7.31 | 742% | 92% |

Threads 1-8 all fit inside the 10 performance cores, so this is near-linear *across P-cores*;
the 4 efficiency cores add only marginal gains beyond 10 threads.

### Wide file — 1M rows x 250 columns (1.25 GB)
| Tool | Time | GB/s | Speedup vs Pandas |
| :--- | :--- | :--- | :--- |
| Pandas (`read_csv` + mean + var) | 5.34 s | 0.24 | 1.0x |
| DuckDB | 1.23 s | 1.02 | 4.3x |
| Polars | 0.70 s | 1.80 | 7.7x |
| **C Fast Parser (14 threads)** | **0.15 s** | **8.23** | **35.1x** |

*Note on scope:* the parser is a fixed-format numeric kernel — it does not do type
inference, quoting, nulls, or scientific notation, and it emits only mean/variance rather
than a queryable table. Part of the speed comes from doing less than a general CSV engine.

### Reproducing
* C parser: `make && ./fast_parser -t 8 data/gigantic_test.csv`
* Baselines: `pip install duckdb polars && python benchmark.py data/gigantic_test.csv`

## Build and Run

This project uses a standard Makefile. The build is compiled with `-O3 -march=native`
for full optimization (this alone is ~4x faster than an unoptimized build).

**To compile:**

make

**To Run:**

Usage: ./fast_parser [-s|--sample] [-t|--threads <num>] filepath

Example: Run with 8 threads and sample variance
./fast_parser -t 8 -s data/dataset.csv

**To Clean:**

make clean
