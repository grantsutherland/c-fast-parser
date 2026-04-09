#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "io_core.h"
#include "parser.h"
#include "engine.h"
#include "compute.h"
#include <pthread.h>

#define MAX_COLUMNS 256

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}


int main(int argc, char*argv[]){
    int use_sample_variance = 0;
    int num_threads = 8;
    char* filepath = NULL;
    for (int i = 1; i < argc; i++) {
        

        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sample") == 0) {
            use_sample_variance = 1;
        }
        
        // Handle Threads Flag
        else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threads") == 0) {

            if (i + 1 < argc) { 
   
                num_threads = atoi(argv[++i]); 
                
                // Guardrails
                if (num_threads < 1) num_threads = 1;
                if (num_threads > 256) num_threads = 256;
            } else {
                fprintf(stderr, "Fatal: -t/--threads requires a number.\n");
                return EXIT_FAILURE;
            }
        }

        else if (argv[i][0] == '-') { 
            fprintf(stderr, "Fatal: Unknown flag '%s'\n", argv[i]);
            return EXIT_FAILURE;
        }
        

        else {
            if (filepath == NULL) {
                filepath = argv[i];
            } else {
                // If filepath is already set, they typed something like `./fast_parser file1.csv file2.csv`
                fprintf(stderr, "Fatal: Multiple filepaths provided.\n");
                return EXIT_FAILURE;
            }
        }
    }

    if (filepath == NULL) {
        fprintf(stderr, "Usage: %s [-s|--sample] [-t|--threads <num>] <filepath>\n", argv[0]);
        return EXIT_FAILURE;
    }
   
    double start_time = get_time();

    

    MappedFile mf = map_file(filepath);

    if(mf.data == NULL){return EXIT_FAILURE;}

    ColumnStats columns[MAX_COLUMNS];

    for(int i =0; i<MAX_COLUMNS;i++){
        init_stats(&columns[i]);
    }


    int max_column_seen = -1;

    run_parallel_parser(mf.data,mf.size,num_threads,columns,&max_column_seen);


    
    



    double end_time = get_time();
    double runtime = end_time - start_time;

    unmap_file(&mf);

    printf("\n--- Engine Execution Complete ---\n");
    printf("Processed in: %.4f seconds\n", runtime);
    if(use_sample_variance){
        printf("Variance Mode: Sample (Bessel's)\n\n");
    }else{
        printf("Variance Mode: Population\n\n");
    }
    
    for(int i = 0; i <= max_column_seen;i++){
        double variance = get_variance(&columns[i],use_sample_variance);
        printf("Column %d | Count: %zu | Mean: %.4f | Variance: %.4f\n", i, columns[i].count, columns[i].mean, variance);
    }

    return EXIT_SUCCESS;

}

