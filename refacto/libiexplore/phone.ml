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


type t = {
  device: Cbindings.ImobileDevice.idevice_t;
  lockdown_client: Cbindings.ImobileDevice.lockdownd_client_t;
  service: Cbindings.ImobileDevice.lockdownd_service_descriptor_t;
  client: Cbindings.ImobileDevice.afc_client_t;
  path: Path.t;
}


let create () = 
  let open Cbindings.ImobileDevice in
  let (let*) = Result.bind in
  let ( |? ) = Result.map_error in
  let ok = Result.ok in
  let* device = Error.device_error |? idevice_new_with_options IDEVICE_LOOKUP_USBMUX in
  let () = at_exit @@ fun () -> idevice_free device in
  let* lockdown_client = Error.lockdown_error |? lockdownd_client_new_with_handshake device in
  let () = at_exit @@ fun () -> lockdownd_client_free lockdown_client in
  let* service = Error.lockdown_error |? lockdownd_start_service lockdown_client in
  let () = at_exit @@ fun () -> lockdownd_service_descriptor_free service in
  let* client = Error.afc_error |? afc_client_new device service in
  let () = at_exit @@ fun () -> afc_client_free client in
  let path = Path.root in
  ok @@ {device; lockdown_client; service; client; path} 

