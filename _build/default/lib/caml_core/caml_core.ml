type path
type afc_connection_t

type idevice_error = [ `Device_error | `Lockckdown_client_error | `Lockdown_service_error | `Afc_error ]

let string_of_idevice_error = function
| `Device_error -> "Device_error : Probabily not connected via usb"
| `Lockckdown_client_error -> "Lockckdown client error"
| `Lockdown_service_error -> "Lockdown service error"
| `Afc_error -> "Afc Error"



external create_path: string list -> path = "caml_create_path"
external new_usb_device: unit -> (afc_connection_t, idevice_error) result = "caml_new_usb_device"
external navigate_device: afc_connection_t -> unit -> unit = "caml_navigate_device"
external afc_connection_free: afc_connection_t -> unit -> unit = "caml_afc_connection_free"