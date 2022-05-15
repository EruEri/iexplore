#define CAML_NAME_SPACE

#include <caml/mlvalues.h>
#include "../c_core/include/path.h"
#include "../c_core/include/idevice.h"
#include "caml/config.h"
#include "caml/misc.h"
#include "caml/memory.h"
#include "caml/alloc.h"

#include "caml_callback.h"

#define Caml_Device_error "Device_error" 
#define Caml_Lockckdown_client_error "Lockckdown_client_error"
#define Caml_Lockdown_service_error "Lockdown_service_error"
#define Caml_Afc_error "Afc_error"


static value Val_path(path_t* path){
    value v = caml_alloc(1, Abstract_tag);
    *((path_t**) Data_abstract_val(v)) = path;
    return v;
}

static path_t* Path_val(value path){
    // return (path_t*) Nativeint_val(path);
    return *((path_t **) Data_abstract_val(path));
}

static value Val_afc_connection(afc_connection_t* client){
    value v = caml_alloc(1, Abstract_tag);
    *((afc_connection_t**) Data_abstract_val(v)) = client;
    return v;
}

static afc_connection_t* Afc_connection_val(value client){
    return *((afc_connection_t**) Data_abstract_val(client));
}

static value Val_Ok(value val){
    CAMLparam1(val);
    CAMLlocal1(ok_result);
    ok_result = caml_alloc(1, 0);
    Store_field(ok_result, 0, val);

    CAMLreturn(ok_result);
}

static value Val_Error(value val){
    CAMLparam1(val);
    CAMLlocal1(error_result);
    error_result = caml_alloc(1, 1);
    Store_field(error_result, 0, val);

    CAMLreturn(error_result);
}

CAMLprim value caml_create_path(value components) {
    CAMLparam1(components);
    path_t* path = new_path(nil, 0);

    while (components != Val_emptylist){
        value head = Field(components, 0);
        add_path_component(path, String_val(head));
        components = Field(components, 1);
    }
    return Val_path(path);
}

CAMLprim value caml_new_usb_device(value unit){
    user_input_t input = get_user_input("Input\n");
    afc_connection_t* connection = new_usb_device();
    switch (connection->result.error) {
    case DEVICE_ERROR:
        afc_connection_free(connection);
        return Val_Error(caml_hash_variant(Caml_Device_error));
    case LOCKDOWN_CLIENT_ERROR:
        afc_connection_free(connection);
        return Val_Error(caml_hash_variant(Caml_Lockckdown_client_error));
    case LOCKDOWN_SERVICE_ERROR:
        afc_connection_free(connection);
        return Val_Error(caml_hash_variant(Caml_Lockdown_service_error));
    case AFC_ERROR:
        afc_connection_free(connection);
        return Val_Error(caml_hash_variant(Caml_Afc_error));
    default:
      return Val_Ok( Val_afc_connection(connection));
    }
}

CAMLprim value caml_afc_connection_free(value client, value unit){
    afc_connection_t* connection = Afc_connection_val(client);
    afc_connection_free(connection);
    return Val_unit;
}

CAMLprim value caml_navigate_device(value client, value unit){
    CAMLparam1(client);
    afc_connection_t* afc_client = Afc_connection_val(client);
    char** directory_info = nil;
    char** info_file;

    path_t* path = new_path(nil, 0);

    while (true) {

    }
    return Val_unit;
}