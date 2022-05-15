#ifndef IDEVICE_H
#define IDEVICE_H

#include <libimobiledevice/afc.h>

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

afc_connection_t* new_usb_device();
void afc_connection_free(afc_connection_t*);

#endif