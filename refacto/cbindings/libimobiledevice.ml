(**********************************************************************************************)
(*                                                                                            *)
(* This file is part of iexplore: An IPhone file explorer                                     *)
(* Copyright (C) 2023 Yves Ndiaye                                                             *)
(*                                                                                            *)
(* iexplore is free software: you can redistribute it and/or modify it under the terms        *)
(* of the GNU General Public License as published by the Free Software Foundation,            *)
(* either version 3 of the License, or (at your option) any later version.                    *)
(*                                                                                            *)
(* iexplore is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;      *)
(* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR           *)
(* PURPOSE.  See the GNU General Public License for more details.                             *)
(* You should have received a copy of the GNU General Public License along with iexplore.     *)
(* If not, see <http://www.gnu.org/licenses/>.                                                *)
(*                                                                                            *)
(**********************************************************************************************)


type idevice_t
type afc_client_t
type lockdownd_client_t
type lockdownd_service_descriptor_t

type idevice_new_with_options = 
  | IDEVICE_LOOKUP_USBMUX
  | IDEVICE_LOOKUP_NETWORK
  | IDEVICE_LOOKUP_PREFER_NETWORK

type idevice_error_t =     
	| IDEVICE_E_INVALID_ARG    
	| IDEVICE_E_UNKNOWN_ERROR  
	| IDEVICE_E_NO_DEVICE      
	| IDEVICE_E_NOT_ENOUGH_DATA
	| IDEVICE_E_SSL_ERROR      
	| IDEVICE_E_TIMEOUT      
  
type lockdownd_error_t = 
  | LOCKDOWN_E_INVALID_ARG                             
  | LOCKDOWN_E_INVALID_CONF                            
  | LOCKDOWN_E_PLIST_ERROR                             
  | LOCKDOWN_E_PAIRING_FAILED                          
  | LOCKDOWN_E_SSL_ERROR                               
  | LOCKDOWN_E_DICT_ERROR                              
  | LOCKDOWN_E_RECEIVE_TIMEOUT                         
  | LOCKDOWN_E_MUX_ERROR                               
  | LOCKDOWN_E_NO_RUNNING_SESSION                      
  | LOCKDOWN_E_INVALID_RESPONSE                        
  | LOCKDOWN_E_MISSING_KEY                             
  | LOCKDOWN_E_MISSING_VALUE                           
  | LOCKDOWN_E_GET_PROHIBITED                          
  | LOCKDOWN_E_SET_PROHIBITED                          
  | LOCKDOWN_E_REMOVE_PROHIBITED                       
  | LOCKDOWN_E_IMMUTABLE_VALUE                         
  | LOCKDOWN_E_PASSWORD_PROTECTED                      
  | LOCKDOWN_E_USER_DENIED_PAIRING                     
  | LOCKDOWN_E_PAIRING_DIALOG_RESPONSE_PENDING         
  | LOCKDOWN_E_MISSING_HOST_ID                         
  | LOCKDOWN_E_INVALID_HOST_ID                         
  | LOCKDOWN_E_SESSION_ACTIVE                          
  | LOCKDOWN_E_SESSION_INACTIVE                        
  | LOCKDOWN_E_MISSING_SESSION_ID                      
  | LOCKDOWN_E_INVALID_SESSION_ID                      
  | LOCKDOWN_E_MISSING_SERVICE                         
  | LOCKDOWN_E_INVALID_SERVICE                         
  | LOCKDOWN_E_SERVICE_LIMIT                           
  | LOCKDOWN_E_MISSING_PAIR_RECORD                     
  | LOCKDOWN_E_SAVE_PAIR_RECORD_FAILED                 
  | LOCKDOWN_E_INVALID_PAIR_RECORD                     
  | LOCKDOWN_E_INVALID_ACTIVATION_RECORD               
  | LOCKDOWN_E_MISSING_ACTIVATION_RECORD               
  | LOCKDOWN_E_SERVICE_PROHIBITED                      
  | LOCKDOWN_E_ESCROW_LOCKED                           
  | LOCKDOWN_E_PAIRING_PROHIBITED_OVER_THIS_CONNECTION 
  | LOCKDOWN_E_FMIP_PROTECTED                          
  | LOCKDOWN_E_MC_PROTECTED                            
  | LOCKDOWN_E_MC_CHALLENGE_REQUIRED                   
  | LOCKDOWN_E_UNKNOWN_ERROR        
  
  
type afc_error_t =             
  | AFC_E_UNKNOWN_ERROR        
  | AFC_E_OP_HEADER_INVALID    
  | AFC_E_NO_RESOURCES         
  | AFC_E_READ_ERROR           
  | AFC_E_WRITE_ERROR          
  | AFC_E_UNKNOWN_PACKET_TYPE  
  | AFC_E_INVALID_ARG          
  | AFC_E_OBJECT_NOT_FOUND     
  | AFC_E_OBJECT_IS_DIR        
  | AFC_E_PERM_DENIED          
  | AFC_E_SERVICE_NOT_CONNECTED
  | AFC_E_OP_TIMEOUT           
  | AFC_E_TOO_MUCH_DATA        
  | AFC_E_END_OF_DATA          
  | AFC_E_OP_NOT_SUPPORTED     
  | AFC_E_OBJECT_EXISTS        
  | AFC_E_OBJECT_BUSY          
  | AFC_E_NO_SPACE_LEFT        
  | AFC_E_OP_WOULD_BLOCK       
  | AFC_E_IO_ERROR             
  | AFC_E_OP_INTERRUPTED       
  | AFC_E_OP_IN_PROGRESS       
  | AFC_E_INTERNAL_ERROR       
  | AFC_E_MUX_ERROR            
  | AFC_E_NO_MEM               
  | AFC_E_NOT_ENOUGH_DATA      
  | AFC_E_DIR_NOT_EMPTY        
  | AFC_E_FORCE_SIGNED_TYPE    

external idevice_new_with_options : ?udid:string -> idevice_new_with_options -> (idevice_t, idevice_error_t) result 
  = "caml_idevice_new_with_options"

external lockdownd_client_new_with_handshake : ?label:string -> idevice_t -> (lockdownd_client_t, lockdownd_error_t) result = 
  "caml_lockdownd_client_new_with_handshake"

external lockdownd_start_service : ?identifier:string -> lockdownd_client_t -> (lockdownd_service_descriptor_t, lockdownd_error_t) result = 
  "caml_lockdownd_start_service"

external afc_client_new : idevice_t -> lockdownd_service_descriptor_t -> (afc_client_t, afc_error_t) result = 
  "caml_afc_client_new"

external idevice_free : idevice_t -> unit = "caml_idevice_free"

external lockdownd_client_free : lockdownd_client_t -> unit = "caml_lockdownd_client_free"

external lockdownd_service_descriptor_free : lockdownd_service_descriptor_t -> unit = 
  "caml_lockdownd_service_descriptor_free"

external afc_client_free : afc_client_t -> unit = 
  "caml_afc_client_free"