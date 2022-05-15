#include <stdlib.h>
#include <stdio.h>

#include "caml/alloc.h"
#include "caml/memory.h"
#include "caml/mlvalues.h"
#include "caml/callback.h"

#include "caml_callback.h"

value get_input(const char* message){
    static value* get_input_closure = NULL;
    if (!get_input_closure) get_input_closure = caml_named_value("get_input");
    value input = caml_callback2 (*get_input_closure, caml_copy_string(message), Val_unit);
    return input;
}