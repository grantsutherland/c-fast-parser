#ifndef PARSER_H
#include <stddef.h>
#include "io_core.h"
#define PARSER_H

//Represents single cell in csv
typedef struct Token{
    const char* start;
    size_t length;
}Token;

typedef struct Parser{
    char* current;
    char* end;
    int current_row;
    int current_col;
}Parser;



void init_parser(Parser *p, const MappedFile *mf);

Token next_token(Parser *p);

double fast_atof(Token t);

#endif

