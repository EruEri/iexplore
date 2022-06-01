#include <libimobiledevice/afc.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/idevice.h"
#include "../include/util.h"


afc_connection_t* new_usb_device(){
    idevice_t device = nil;
    lockdownd_client_t lockdown_client  = nil;
    lockdownd_service_descriptor_t service_descriptor = nil;
    afc_client_t client = nil;

    afc_connection_t* result = malloc(sizeof(afc_connection_t));
    if (!result) exit_with_message(1, malloc_msg_error);

    if (idevice_new_with_options(&device, 
    nil, IDEVICE_LOOKUP_USBMUX) != IDEVICE_E_SUCCESS) {
        result->result.error = DEVICE_ERROR;
        result->device = nil;
        result->service_descriptor = nil;
        result->lockdown_client = nil;
        return result;
    }

    if (lockdownd_client_new_with_handshake(device, 
    &lockdown_client, AFC_SERVICE_NAME) != LOCKDOWN_E_SUCCESS) {
        idevice_free(device);
        
        result->result.error = LOCKDOWN_CLIENT_ERROR;
        result->device = nil;
        result->service_descriptor = nil;
        result->lockdown_client = nil;
        return result;
    }

    if (lockdownd_start_service(lockdown_client, AFC_SERVICE_NAME,
                                &service_descriptor) != LOCKDOWN_E_SUCCESS) {
        idevice_free(device);
        lockdownd_client_free(lockdown_client);
        
        result->result.error = LOCKDOWN_SERVICE_ERROR;
        result->device = nil;
        result->service_descriptor = nil;
        result->lockdown_client = nil;
        return result;       
    }
    if (afc_client_new(device, service_descriptor, &client) != AFC_E_SUCCESS) {
        idevice_free(device);
        lockdownd_client_free(lockdown_client);
        lockdownd_service_descriptor_free(service_descriptor);
        
        result->device = nil;
        result->service_descriptor = nil;
        result->lockdown_client = nil;
        return result;
    }

    
    result->result.client = client;
    result->lockdown_client = lockdown_client;
    result->device = device;
    result->service_descriptor = service_descriptor;
    result->result.error = NO_ERROR;

    return result;
}

void afc_connection_free(afc_connection_t * connection){
    if (connection->device) idevice_free(connection->device);
    if (connection->lockdown_client) lockdownd_client_free(connection->lockdown_client);
    if (connection->service_descriptor) lockdownd_service_descriptor_free(connection->service_descriptor);
    if (connection->result.client) afc_client_free(connection->result.client);
    
    free(connection);
}

void afc_print_dict(char** directory){
    int i = 0;
    char* folder;
    while( (folder = directory[i]) ) printf("%02d : %s\n", ++i, folder);
}

afc_file_t** afc_file_dir_of_string_dir(path_t* path){
    printf("Not implemented yet\n");
    exit(-1);
}

afc_file_t* new_file_info(afc_client_t client, path_t* path, char* last_component){
    char** file_info = nil;
    if (add_path_component(path, last_component) == -1) return nil;

    const char* str_path = malloc_to_string(path);
    remove_last_path_component(path);

    if (!str_path) return nil;
    if (afc_get_file_info(client, str_path , &file_info) != AFC_E_SUCCESS) {
        free_malloc_string((char *) str_path);
        return nil;
    }
    free_malloc_string((char *) str_path);

    afc_file_t* file = malloc(sizeof(afc_file_t));
    if (!file) return nil;

    file->st_size = atoi(file_info[1]);
    file->st_blocks = atoi(*(file_info + 3));
    file->st_nlink = atoi(file_info[5]);
    file->file_type = make_str_copy(file_info[7]);
    file->_st_mtime = atoll(file_info[9]);
    file->_st_birthtime = atoll(file_info[11]);
    file->filename = make_str_copy(last_component);

    afc_dictionary_free(file_info);
    return file;
}

int copy_intern_file(afc_connection_t* connection, uint64_t iphone_fd, const char* output_path){
    FILE* output = fopen(output_path, "ab+");
    if (!output) return -1;
    afc_client_t client = connection->result.client;
    const unsigned int BYTES_TO_READ = 500000;
    unsigned int bytes_read = 0;
    char buffer[BYTES_TO_READ];

    do {
        if (afc_file_read(client, iphone_fd, buffer, BYTES_TO_READ, &bytes_read) != AFC_E_SUCCESS) {
            fclose(output);
            return -1;
        }
        fwrite(buffer, sizeof(char), bytes_read, output);
    }while (bytes_read == BYTES_TO_READ);

    fclose(output);
    return 0;
}




void free_file_info(afc_file_t* file){
    free(file->filename);
    free(file->file_type);
    free(file);
}