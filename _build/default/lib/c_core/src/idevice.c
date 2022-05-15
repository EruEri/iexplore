#include <libimobiledevice/afc.h>
#include <stdlib.h>

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