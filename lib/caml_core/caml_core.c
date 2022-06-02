#define CAML_NAME_SPACE

#include <stdio.h>
#include <stdint.h>


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
    char** directory_info;
    char** info_file;

    path_t* path = new_path(nil, 0);
    int i = 0;
    while (true) {
        if (i == 5) exit_with_message(1, "Read dictonary error limit\n");
        directory_info = nil;
        char* str_path = (char*) malloc_to_string(path);
        if (!path) exit_with_message(1, "Unable to display the path");

        if (afc_read_directory(afc_client->result.client,
         str_path, &directory_info) != AFC_E_SUCCESS) {
            free_malloc_string(str_path);
            remove_last_path_component(path);
            printf("directory error\n");
            printf("Maybe it's a file\n");
            i += 1;
            continue;
        }
        i = 0;
        printf("PATH : %s\n", str_path);
        free_malloc_string((char *) str_path);
        afc_print_dict(directory_info);
        printf("Which folder do you want to enter?\n");
        printf("i : info - q : quit - c : copy\n");
        user_input_t user_input = get_user_input("");
        switch (user_input.choice) {
        case NUMBER:{
            char* component = directory_info[user_input.value.number_choose -1];
            add_path_component(path, (const char*) component); 
            continue;
        }
        case COPY :{
            int file_choose = read_int("Which file (pick number)\n> ", "Please enter a number\n");
            const char* output_path = read_line();
            char* component = directory_info[file_choose -1];
            add_path_component(path, component);
            str_path = (char*) malloc_to_string(path);
            remove_last_path_component(path);
            if (!str_path) {
                printf("Unable to create path\n");
                continue;
            }
            uint64_t fd = 0;
            if (afc_file_open(afc_client->result.client, str_path, AFC_FOPEN_RDONLY, &fd) != AFC_E_SUCCESS) {
                printf("Unable to open the selected file\n");
                free_malloc_string(str_path);
                continue;
            }

            if (copy_intern_file(afc_client, fd, output_path) != 0) {
                printf("Error while copying the file\n");
                free_malloc_string(str_path);
                afc_file_close(afc_client->result.client, fd);
                continue;
            }

            free_malloc_string(str_path);
            afc_file_close(afc_client->result.client, fd);
            continue;
            }
            
        case INFO :{
            int file_choose = read_int("Which file (pick number)\n> ", "Please enter a number\n");
            char* component = directory_info[file_choose -1];
            add_path_component(path, component);
            str_path = (char*) malloc_to_string(path);
            remove_last_path_component(path);
            if (!str_path) {
                printf("Unable to create path\n");
                continue;
            }
            if (afc_get_file_info(afc_client->result.client, 
            str_path, &info_file) != AFC_E_SUCCESS) {
                printf("Unable to fetch file Info\nMaybe it's a diretory\n");
                free_malloc_string(str_path);
            }
            afc_print_dict(info_file);
            continue;
        }
        case QUIT : 
            exit_with_message(0, "Programm end\n");
        }
    }
    return Val_unit;
}