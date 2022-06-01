#ifndef CALL_CALLBACK_H
#define CALL_CALLBACK_H

#include "caml/mlvalues.h"

typedef enum{
    NUMBER,
    COPY,
    INFO,
    QUIT
} user_choice_t;
typedef struct{
    user_choice_t choice;
    union {
        int number_choose;
    }value;
} user_input_t;

user_input_t get_user_input(const char* message);

const char* read_line();

int read_int(const char* message, const char* error_message);

#endif