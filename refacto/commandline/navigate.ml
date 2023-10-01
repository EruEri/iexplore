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

let name = "navigate"

type cmd = unit


let cmd_term run = 
  let combine () = run @@ ()
  in
  Term.(const @@ combine ())


let doc = "Navigate"

let man = [
  `S Manpage.s_description;
  `P "Navigate into iPhone files"
]

let navigate run = 
  let info = 
    Cmd.info ~doc ~man  name
  in
  Cmd.v info (cmd_term run)

let run cmd = 
  let () = cmd in
  let phone = 
    match Libiexplore.Phone.create () with
    | Ok phone -> phone
    | Error e -> raise @@ Libiexplore.Error.exn_explore_error e
  in
  let () = Libiexplore.Repl.repl phone in
  ()


let eval () = Cmd.eval (navigate run)