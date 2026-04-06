#include <stdio.h>
#include <stdlib.h>
#include "io_core.h"


int process_data(char* filepath){
    printf("Opening file at %s\n", filepath);
    FILE *infile = fopen(filepath, "r");
    if(infile == NULL){
        fprintf(stderr, "Error accessing file at %s\n", filepath);
        return -1;
    }
    printf("File successfully opened\n");

    //ADD all preparation here

    fclose(infile);
    printf("File closed safely\n");
    return 0;

}
