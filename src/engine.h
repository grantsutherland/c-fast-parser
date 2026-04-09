#ifndef ENGINE_H
#define ENGINE_H


#include "compute.h"


void run_parallel_parser(const char *file_map, size_t file_size, int num_threads, ColumnStats *out_stats, int *out_max_col);

#endif
