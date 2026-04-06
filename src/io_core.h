#ifndef IO_CORE_H
#define IO_CORE_H

typedef struct{
    char* data;
    size_t size;
    int FileDescriptor;

}MappedFile;

MappedFile map_file(const char* filepath);
void unmap_file(MappedFile* mf);

#endif
