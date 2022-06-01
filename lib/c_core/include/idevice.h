#ifndef IDEVICE_H
#define IDEVICE_H

#include <libimobiledevice/afc.h>
#include "path.h"
typedef enum {
    NO_ERROR,
    DEVICE_ERROR,
    LOCKDOWN_CLIENT_ERROR,
    LOCKDOWN_SERVICE_ERROR,
    AFC_ERROR,
} device_error_t;

typedef struct {
    device_error_t error;
    afc_client_t client;
}afc_result_t;

typedef struct{
    idevice_t device;
    lockdownd_client_t lockdown_client;
    lockdownd_service_descriptor_t service_descriptor;
    afc_result_t result;
} afc_connection_t;


typedef struct {
    char* file_type;
    off_t st_size;
    blksize_t st_blocks;
    nlink_t st_nlink;
    unsigned long _st_mtime;
    unsigned long _st_birthtime;
    char* filename;
} afc_file_t;

afc_connection_t* new_usb_device();
void afc_connection_free(afc_connection_t*);
void afc_print_dict(char** directory);

afc_file_t* new_file_info(afc_client_t, path_t*, char*);

int copy_intern_file(afc_connection_t*, uint64_t iphone_fd, const char* output_file);

void free_file_info(afc_file_t*);
#endif