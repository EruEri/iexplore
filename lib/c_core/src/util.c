#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/util.h"


void exit_with_message(int code, const char* message){
    if (message) {
        printf("%s\n", message);
    }
    exit(code);
}

char* make_str_copy(const char* source){
    size_t len = strlen(source) + 1;
    char* copy = malloc(len);
    return copy ? strcpy(copy, source) : nil;
}

int max(int a, int b){
    return a > b ? a : b;
}

bool_t str_eql(char * str1, char* str2){
    return strcmp(str1, str2) == 0;
}