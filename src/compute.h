#ifndef COMPUTE_H
#include <stddef.h>
#define COMPUTE_H

typedef struct ColumnStats
{
    size_t count;
    double mean;
    double M2;
}ColumnStats;

void init_stats(ColumnStats *stats);
void update_stats(ColumnStats *stats, double new_value);
double get_variance(const ColumnStats *stats,int use_sample_variance);
void combine_stats(ColumnStats *global, ColumnStats *local);


#endif
