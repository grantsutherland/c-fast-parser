#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


void init_parser(Parser *p, const MappedFile *mf){
    p->current = mf->data;
    p->end = p->current + mf->size;

    p->current_row =0;
    p->current_col = 0;
}


Token next_token(Parser *p){
    if(p->current >= p->end){
        Token token = {NULL, 0};
        return token;
    }
    char* start = p->current;

    while(p->current < p->end && *p->current != '\n' && *p->current != ','){
        p->current++;
    }

    size_t length = p->current - start;

    if (length > 0 && start[length - 1] == '\r') {
        length--;
    }

    if(p->current < p->end){
        if(*p->current == ','){
            p->current_col++;
        }else if(*p->current == '\n'){
            p->current_row++;
            p->current_col = 0;
        }
    }

    if(p->current < p->end){
        p->current++;
    }

    Token parsed_token = {start, length};

    return parsed_token;



}

double fast_atof(Token t){
    if(t.length == 0){
        return 0.0;
    }

    double result = 0.0;
    double sign = 1.0;
    size_t idx = 0;

    if(*t.start == '-'){
        sign = -1.0;
        idx++;
    }

    while(idx < t.length && t.start[idx] != '.'){
        result = (result * 10) + (t.start[idx] - '0');
        idx++;
    }
    if(idx != t.length){
        idx++;
        double numerator = 0.0;
        double divisor = 1.0;
        while(idx < t.length){
            numerator = (numerator *10) + (t.start[idx] - '0');
            divisor *= 10.0;
            idx++;
        }
        result += numerator / divisor;
    }
    return result * sign;

}
