#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "io_core.h"



MappedFile map_file(const char* filepath){

    
    MappedFile mf = {NULL, 0, -1};

    //Getting file descripor
    mf.FileDescriptor = open(filepath,O_RDONLY);
    if (mf.FileDescriptor == -1){
        fprintf(stderr, "Error accessing file at %s\n", filepath);
        return mf;
    }

    struct stat buffer;

    int status = fstat(mf.FileDescriptor,&buffer);
    if (status == -1){
        fprintf(stderr, "Error accessing metadata\n");
        close(mf.FileDescriptor);
        mf.FileDescriptor = -1;
        return mf;
    }
    mf.size = buffer.st_size;

    //mmap handling, read only protection, private copy

    mf.data = (char*)mmap(NULL, mf.size,PROT_READ,MAP_PRIVATE,mf.FileDescriptor,0);

    if(mf.data == MAP_FAILED){
        fprintf(stderr,"Memory mapping failed\n");
        close(mf.FileDescriptor);
        mf.FileDescriptor = -1;
        mf.data = NULL;
        mf.size = 0;
        return mf;
    }
    return mf;


}

void unmap_file(MappedFile* mf){
    if(mf->data != NULL && mf->data != MAP_FAILED){
        munmap(mf->data, mf->size);
        mf->data = NULL;
    }else{
        fprintf(stderr, "munmap failed\n");
    }
    if(mf->FileDescriptor != -1){
        close(mf->FileDescriptor);
        mf->FileDescriptor = -1;
    }

    mf->size = 0;

}

