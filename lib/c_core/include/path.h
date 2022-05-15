#ifndef PATH_H
#define PATH_H

#include "util.h"

typedef struct {
    char root;
    char** components;
    int lenght;
    int max_reached_len;
} path_t;


path_t* new_path(char** components, int argc);

int add_path_component(path_t*, const char*);

void remove_last_path_component(path_t*);

void free_path(path_t*);

bool_t has_component(path_t*);

void show_path(path_t*);

const char* malloc_to_string(path_t*);

void free_malloc_string(char*);

#endif