#include "compute.h"
#include <stdio.h>
#include <stdlib.h>


void init_stats(ColumnStats *stats){
    stats->count = 0;
    stats->mean = 0;
    stats->M2 = 0;
}

void update_stats(ColumnStats *stats, double new_value){
    stats->count++;
    double delta = new_value - stats->mean;
    stats->mean += delta / stats->count;
    double delta2 = new_value - stats->mean;
    stats->M2 += delta * delta2;
}

double get_variance(const ColumnStats *stats, int use_sample_variance){
    if(stats->count < 2){
        return 0.0;
    }

    if(use_sample_variance){
        return stats->M2 / (stats->count - 1);
    }else{
        return stats->M2 / stats->count;
    }


}