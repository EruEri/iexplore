#ifndef UTIL_H
#define UTIL_H

#define nil NULL
#define true 1
#define false 0
#define malloc_msg_error "malloc error"

typedef char bool_t;

void exit_with_message(int code, const char* message);
int max(int a, int b);
char* make_str_copy(const char* source);
bool_t str_eql(char*, char*);

#endif