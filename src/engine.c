#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "engine.h"
#include "parser.h"

#define MAX_COLUMNS 256


typedef struct{
    int thread_id;
    const char* start_ptr;
    const char* soft_limit;
    const char* file_end;
    int max_col_seen;

    ColumnStats local_stats[MAX_COLUMNS];
}Thread_Data;

void* parse_chunk(void *arg){
    Thread_Data *thread = (Thread_Data*)arg;
    for(int i = 0; i<MAX_COLUMNS;i++){
        init_stats(&thread->local_stats[i]);
    }
    thread->max_col_seen = -1;

    if(thread->thread_id > 0){
        if (*(thread->start_ptr - 1) != '\n') {
            while (thread->start_ptr < thread->soft_limit && *thread->start_ptr != '\n') {
                thread->start_ptr++;
            }
            if (*thread->start_ptr == '\n') {
            thread->start_ptr++; 
        }
        }  
    }
    Parser p;
    p.current = thread->start_ptr;
    p.current_col = 0;
    p.end = thread->file_end;

    while(p.current < thread->file_end){
    
        int active_col = p.current_col;

        Token t = next_token(&p);
        if (t.length == 0) {
            break; // EOF reached
        }

        if(active_col < MAX_COLUMNS){
            double value = fast_atof(t);
            update_stats(&thread->local_stats[active_col], value);

            if (active_col > thread->max_col_seen){
                thread->max_col_seen = active_col;
            }
        }
        if(p.current >= thread->soft_limit && p.current_col ==0){
            break;
        }

    }
    return NULL;
    
}

void run_parallel_parser(const char *file_map, size_t file_size, int num_threads, ColumnStats *out_stats, int *out_max_col){
    size_t chunk_size = file_size / num_threads;
    pthread_t threads[num_threads];
    Thread_Data thread_data[num_threads];

    for(int i = 0; i<num_threads;i++){
        thread_data[i].thread_id = i;
        thread_data[i].start_ptr = file_map + (i * chunk_size);
        thread_data[i].file_end = file_map + file_size;
        
        if(i == num_threads - 1){
            thread_data[i].soft_limit = file_map + file_size;

        }else{
            thread_data[i].soft_limit = thread_data[i].start_ptr + chunk_size;
        }
        pthread_create(&threads[i],NULL,parse_chunk,&thread_data[i]);
        
    }

    int global_max_col = -1;

    for(int i = 0; i<num_threads;i++){
        pthread_join(threads[i], NULL);
        if(thread_data[i].max_col_seen > global_max_col){
            global_max_col = thread_data[i].max_col_seen;
        }

        for(int col = 0; col <= thread_data[i].max_col_seen; col++){
            combine_stats(&out_stats[col],&thread_data[i].local_stats[col]);
        }
    }

    *out_max_col = global_max_col;


}

