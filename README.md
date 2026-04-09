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

Benchmarks were run on an Apple Silicon Mac (8 Cores) against a 100-Million Row CSV file (~5GB).

### Pandas vs. C Fast Parser (100M Rows)
| Tool / Language | Execution Time | Speedup |
| :--- | :--- | :--- |
| **Python (Pandas)** | ~9.07 seconds | 1.0x Baseline |
| **Fast Parser (C)** | **~2.01 seconds** | **~4.5x Faster** |

### CPU Core Scaling
The architecture is lock-free, meaning additional CPU cores result in near-linear performance gains. 

| Threads | Execution Time (100M Rows) | CPU Utilization |
| :---: | :--- | :--- |
| **1 Core** | 13.65s *(Cold Cache)*| 71% |
| **2 Cores** | 4.98s | 199% |
| **4 Cores** | 2.78s | 372% |
| **8 Cores** | ~2.01s | 528% |

### Wide-Data Processing
Tested against a dataset of 1,000,000 rows x 250 columns to verify stack-memory efficiency.
* **Baseline (Pandas):** ~9.52 seconds
* **C Fast-Parser:** ~1.28 seconds
* **Speedup over Pandas:** ~7.25x Faster

## Build and Run

This project uses a standard Makefile.

**To compile:**

make

**To Run:**

Usage: ./fast_parser [-s|--sample] [-t|--threads <num>] filepath

Example: Run with 8 threads and sample variance
./fast_parser -t 8 -s data/dataset.csv

**To Clean:**

make clean
