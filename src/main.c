#include <stdio.h>
#include <stdlib.h>
#include "io_core.h"

int main(int argc, char*argv[]){
    if (argc != 2){
        fprintf(stderr, "Invalid parameter count, usage: %s {filepath} \n", argv[0]);
        return EXIT_FAILURE;
    }

    char *filepath = argv[1];

    printf("Parsing data at filepath: %s\n", filepath);

    if(process_data(filepath) != 0){
        fprintf(stderr, "Error processing data at filepath: %s \n", filepath);
        return EXIT_FAILURE;
        

    }else{
        printf("Success: data at %s successfully parsed\n", filepath);
    }

}
