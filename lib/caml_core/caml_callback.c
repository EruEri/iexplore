#include <stdlib.h>
#include <stdio.h>

#include "caml/alloc.h"
#include "caml/memory.h"
#include "caml/mlvalues.h"
#include "caml/callback.h"

#include "caml_callback.h"

#define Input_Number "Number"
#define Input_Quit "Quit"
#define Input_Info "Info"
#define Input_Copy "Copy"

static value get_input(const char* message){
    static const value* get_input_closure = NULL;
    if (!get_input_closure) get_input_closure = caml_named_value("get_input");
    value input = caml_callback2 (*get_input_closure, caml_copy_string(message), Val_unit);
    return input;
}

int read_int(const char* message, const char* error_message){
    static const value* read_int_safe_closure = NULL;
    if (!read_int_safe_closure) read_int_safe_closure = caml_named_value("read_int");
    value read = caml_callback3(*read_int_safe_closure, caml_copy_string(message), caml_copy_string(error_message), Val_unit);
    return Int_val(read);
}

user_input_t get_user_input(const char* message){
    value input = get_input(message);
    user_input_t user_input;
    if (Is_block(input) && Field(input, 0) == caml_hash_variant(Input_Number) ) {
        user_input.choice = NUMBER;
        user_input.value.number_choose = Int_val(Field(input, 1));
    } else if (input == caml_hash_variant(Input_Info)) {
        user_input.choice = INFO;
    } else if (input == caml_hash_variant(Input_Copy)) {
        user_input.choice = COPY;
    } else {
        user_input.choice = QUIT;
    }
    return user_input;
}