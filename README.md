# C Fast Parser 

A high performance CSV parser written in C

Bypasses the standard I/O bottlenecks of fopen and fread by utilizing POSIX system calls (mmap) to project files directly into virtual memory. It is designed for environments that need to ingest massive, high-dimensional datasets—like single-cell matrices or gigabytes of market tick data—into machine learning pipelines faster than standard Python libraries allow.

## Core Features
* Zero-Copy Memory Mapping: Uses mmap to read files directly from disk to RAM.
* Custom Pointer Arithmetic: Implements an optimized ASCII-to-float parser.
* Welford's Algorithm: Calculates running mean and variance in a single pass.
* Zero Dependencies: Requires no external libraries, in solely C.

## Architecture
* io_core: Handles file descriptors, fstat, and memory mapping.
* parser: The hot-loop that executes raw pointer traversal.
* compute: The mathematical backend.

## Build and Run
This project uses a standard Makefile. 

**To compile:**

make

**To Run:**

./fast_parser <path_to_csv>

**To Clean:**

make clean
