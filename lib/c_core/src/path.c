#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/path.h"
#include "../include/util.h"


path_t* new_path(char** components, int argc){
    path_t* path = malloc(sizeof(path_t));
    if (path == nil) exit_with_message(1, malloc_msg_error);
    path->root = '/';
    if (components == nil){
        char** path_components = malloc(0);
        if (!path_components) exit_with_message(1, malloc_msg_error);
        path->components = path_components;
        path->lenght = 0;
        path->max_reached_len = 0;
    } else {
        char** components_copy = malloc(sizeof(char*) * argc);
        if (!components_copy) exit_with_message(1, malloc_msg_error);
        for (int i = 0; i < argc; i++) {
            char* component = components[i];
            size_t component_len = strlen(component) + 1;
            char* component_copy = malloc(component_len);
            if (!component_copy) exit_with_message(1, malloc_msg_error);
            strcpy(component_copy, component);
            components_copy[i] = component_copy;
        }
        path->components = components_copy;
        path->lenght = argc;
        path->max_reached_len = argc;
    }
    return path;
}

void free_path(path_t * path){
    for (int i = 0; i < path->lenght; i++) {
        char* component = path->components[i];
        free(component);
    }
    free(path->components);
    free(path);
}

bool_t need_realloc(path_t* path){
    return path->max_reached_len <= path->lenght;
}

int add_path_component(path_t* path, const char* component){

    if (strcmp(component, ".") == 0) return 0;
    else if (strcmp(component, "..") == 0) {
        remove_last_path_component(path);
        return 0;
    }

    char* copy = make_str_copy(component);
    if ( !copy ) return -1;

    if (need_realloc(path)) {
        char** realloced = realloc(path->components, sizeof(char*) * (path->lenght + 1) );
        if (!realloced) return -1;
        path->components = realloced;
    }
    path->components[path->lenght] = copy;
    path->lenght = path->lenght + 1;
    path->max_reached_len = max(path->max_reached_len, path->lenght);
    return 0;
}

void remove_last_path_component(path_t* path){
    if (!has_component(path)) return ;
    path->lenght--;
    free(path->components[path->lenght]);
    path->components[path->lenght] = nil;
}

bool_t has_component(path_t* path){
    return path->lenght > 0;
}

void show_path(path_t* path){
    printf("/");
    for (int i = 0; i < path->lenght; i++) {
        printf("%s", path->components[i]);
        if (i <= path->lenght -1) printf("/");
    }
}