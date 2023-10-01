
#include <stddef.h>
#include <stdlib.h>
#include "libimobiledevice/libimobiledevice.h"
#include "libimobiledevice/afc.h"
#include "caml/memory.h"
#include "caml/misc.h"
#include "caml/mlvalues.h"
#include "caml/alloc.h"


size_t directory_length(const char** directory) {
    size_t i = 0;
    for (const char* elt = directory[0]; elt != NULL; directory += 1) {
        i += 1;
    }
    return i;
}

 

value init_directory(const char** directory) {
    CAMLparam0();
    CAMLlocal1(caml_directory);
    caml_directory = caml_alloc_array(caml_copy_string, directory);
    CAMLreturn(caml_directory);
}

static value val_ok(value affect) {
    CAMLparam1(affect);
    CAMLlocal1(caml_variable);
    caml_variable = caml_alloc(1, 0);
    Field(caml_variable, 0) = affect;
     CAMLreturn(caml_variable);
}

static value val_err(value affect) {
    CAMLparam1(affect);
    CAMLlocal1(caml_variable);
    caml_variable = caml_alloc(1, 1);
    Field(caml_variable, 0) = affect;
    CAMLreturn(caml_variable);
}

/**
    Transform an **idevice_error_t** to ocaml idevice_error_t
    Should not be the IDEVICE_E_SUCCESS case
*/
static value val_idevice_error(idevice_error_t e) {
    switch (e) {
    case IDEVICE_E_SUCCESS:
        return 0;
    default:
        return abs(e + 1);
    }
}

/**
    Transform an **lockdownd_error_t** to ocaml lockdownd_error_t
    Should not be the LOCKDOWN_E_SUCCESS case
*/
static value val_lockdownd_error(lockdownd_error_t e) {
    int value = abs(e + 1);
    switch (e) {
    case LOCKDOWN_E_SUCCESS:
        return 0;
    case LOCKDOWN_E_UNKNOWN_ERROR:
        // before last
        return abs(LOCKDOWN_E_MC_CHALLENGE_REQUIRED + 1 + 1);
    default:
        return value;
    }
}


static value val_afc_error(afc_error_t e) {
    int value = abs(e - 1);
    switch (e) {
    case AFC_E_SUCCESS:
        return 0;
    case AFC_E_FORCE_SIGNED_TYPE:
        return abs(AFC_E_DIR_NOT_EMPTY);
    default:
        return value;
    }
}

#define Val_define(type) \
    static value val_##type(type v##type) {\
        value v = caml_alloc(1, Abstract_tag);\
        *((type*) Data_abstract_val(v)) = v##type;\
        return v;\
    }\
    static type type##_val(value v##type) {\
        return *((type*) Data_abstract_val(v##type));\
    }

Val_define(idevice_t)
Val_define(lockdownd_client_t)
Val_define(lockdownd_service_descriptor_t)
Val_define(afc_client_t)

CAMLprim value caml_idevice_free(value caml_device) {
    CAMLparam1(caml_device);
    idevice_t d = idevice_t_val(caml_device);
    idevice_free(d);
    CAMLreturn(Val_unit);
}

CAMLprim value caml_lockdownd_client_free(value caml_lockdown_client) {
    CAMLparam1(caml_lockdown_client);
    lockdownd_client_t client = lockdownd_client_t_val(caml_lockdown_client);
    lockdownd_client_free(client);
    CAMLreturn(Val_unit);
}

CAMLprim value caml_lockdownd_service_descriptor_free(value caml_lockdownd_service_descriptor) {
    CAMLparam1(caml_lockdownd_service_descriptor);
    lockdownd_service_descriptor_t service = lockdownd_service_descriptor_t_val(caml_lockdownd_service_descriptor);
    lockdownd_service_descriptor_free(service);
    CAMLreturn(Val_unit);
}

CAMLprim value caml_afc_client_free(value caml_afc_client) {
    CAMLparam1(caml_afc_client);
    afc_client_t client = afc_client_t_val(caml_afc_client);
    afc_client_free(client);
    CAMLreturn(Val_unit);
}



CAMLprim value caml_idevice_new_with_options(value udid_opt, value option) {
    CAMLparam2(udid_opt, option);
    CAMLlocal2(caml_string, caml_block);
    enum idevice_options c_opt = (Int_val(option) + 1) << 1;
    const char* udid = NULL;
    if (Is_block(udid_opt)) {
        caml_string = Some_val(udid_opt);
        udid = String_val(caml_string);
    }
    idevice_t device = NULL;
    idevice_error_t status;
    status = idevice_new_with_options(&device, udid, c_opt);
    if (status == IDEVICE_E_SUCCESS) {
        caml_block = val_ok(val_idevice_t(device));
        CAMLreturn(caml_block);
    } else {
        caml_block = val_err(val_idevice_error(status));
        CAMLreturn(caml_block);
    }
}

CAMLprim value caml_lockdownd_client_new_with_handshake(value caml_label, value caml_device) {
    CAMLparam2(caml_label, caml_device);
    CAMLlocal2(caml_string, caml_block);
    idevice_t device = idevice_t_val(caml_device);
    lockdownd_client_t lockdown_client = NULL;
    const char* label = AFC_SERVICE_NAME;
    if (Is_some(caml_label)) {
        caml_string = Some_val(caml_label);
        label = String_val(caml_string);
    }
    lockdownd_error_t status;
    status = lockdownd_client_new_with_handshake(device, &lockdown_client, label);
    if (status == LOCKDOWN_E_SUCCESS) {
        caml_block = val_ok(val_lockdownd_client_t(lockdown_client));
        CAMLreturn(caml_block);
    } else {
        caml_block = val_err(val_lockdownd_error(status));
        CAMLreturn(caml_block);
    }
}

CAMLprim value caml_lockdownd_start_service(value caml_identifier, value caml_lockdown_client) {
    CAMLparam2(caml_identifier, caml_lockdown_client);
    CAMLlocal2(caml_string, caml_block);
    lockdownd_client_t client = lockdownd_client_t_val(caml_lockdown_client);
    lockdownd_service_descriptor_t service = NULL;
    const char* identifier = AFC_SERVICE_NAME;
    if (Is_some(caml_identifier)) {
        caml_string = Some_val(caml_identifier);
        identifier = String_val(caml_string);
    }
    lockdownd_error_t status;
    status = lockdownd_start_service(client, identifier, &service);
    if (status == LOCKDOWN_E_SUCCESS) {
        caml_block = val_ok(val_lockdownd_service_descriptor_t(service));
        CAMLreturn(caml_block);
    } else {
        caml_block = val_err(val_lockdownd_error(status));
        CAMLreturn(caml_block);
    }
}

CAMLprim value caml_afc_client_new(value caml_device, value caml_service) {
    CAMLparam2(caml_device, caml_service);
    CAMLlocal1(caml_block);
    idevice_t device = idevice_t_val(caml_device);
    lockdownd_service_descriptor_t service = lockdownd_service_descriptor_t_val(caml_service);
    afc_error_t status;
    afc_client_t client = NULL;
    status = afc_client_new(device, service, &client);
    if (AFC_E_SUCCESS == status) {
        caml_block = val_ok(val_afc_client_t(client));
        CAMLreturn(caml_block);
    } else {
        caml_block = val_err(val_afc_error(status));
        CAMLreturn(caml_block);
    }
}

/**

*/
CAMLprim value caml_afc_read_directory(value caml_afc_client, value caml_str_path) {
    CAMLparam2(caml_afc_client, caml_str_path);
    CAMLlocal2(caml_result, caml_directory);
    char** directory_info = NULL; 
    const char* path = String_val(caml_str_path);
    afc_client_t client = afc_client_t_val(caml_afc_client);
    afc_error_t status;
    status = afc_read_directory(client, path, &directory_info);
    if (status == AFC_E_SUCCESS) {
        caml_directory = init_directory((const char**) directory_info);
        caml_result = val_ok(caml_directory);
        afc_dictionary_free(directory_info);
        CAMLreturn(caml_result);
    } else {
        caml_result = val_err(val_afc_error(status));
        CAMLreturn(caml_result);
    }
}

CAMLprim value caml_afc_get_file_info(value caml_afc_client, value caml_str_path) {
    CAMLparam2(caml_afc_client, caml_str_path);
    CAMLlocal2(caml_result, caml_directory);
    char** directory_info = NULL; 
    const char* path = String_val(caml_str_path);
    afc_client_t client = afc_client_t_val(caml_afc_client);
    afc_error_t status;
    status = afc_get_file_info(client, path, &directory_info);
    if (status == AFC_E_SUCCESS) {
        caml_directory = init_directory((const char**) directory_info);
        caml_result = val_ok(caml_directory);
        afc_dictionary_free(directory_info);
        CAMLreturn(caml_result);
    } else {
        caml_result = val_err(val_afc_error(status));
        CAMLreturn(caml_result);
    }
}