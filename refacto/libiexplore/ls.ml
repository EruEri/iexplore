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

open Cmdliner

let name = "ls"

type cmd = unit


let cmd_term phone run = 
  let combine () = run phone ()
  in
  Term.(const @@ combine ())


let doc = "ls - list directory content"

let man = [
  `S Manpage.s_description;
  `P "List file within the directory"
]

let navigate phone run = 
  let info = 
    Cmd.info ~doc ~man  name
  in
  Cmd.v info (cmd_term phone run)

let run phone cmd = 
  let () = cmd in
  let (let*) = Result.bind in
  let ( |? ) f m = Result.map_error m f in
  let ok = Result.ok in
  let* directory = Phone.read_path phone |? (fun e -> Printexc.to_string @@ Error.exn_explore_error @@ Error.afc_error e) in
  let () = 
    Array.iter (Printf.printf "%s\n") directory 
  in 
  ok ()

let command phone = navigate phone run


let eval argv phone = Cmd.eval_result ~argv (command phone)
