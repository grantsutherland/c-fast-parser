#include <stdio.h>
#include <stdlib.h>
#include "io_core.h"

int main(int argc, char*argv[]){
    if (argc != 2){
        fprintf(stderr, "Invalid parameter count, usage: %s {filepath} \n", argv[0]);
        return EXIT_FAILURE;
    }

    char *filepath = argv[1];

    MappedFile mf = map_file(filepath);

    if(mf.data == NULL){return EXIT_FAILURE;}

    printf("Previewing file\n");

    for(int i = 0; i<mf.size;i++){
        putchar(mf.data[i]);
    }

    printf("End of preview, total size: %zu bytes\n", mf.size);

    unmap_file(&mf);
    return EXIT_SUCCESS;

}
