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

let name = "clear"

let clear_console = "\u{001B}[2J"

type cmd = unit


let cmd_term phone run = 
  let combine () = run phone ()
  in
  Term.(const @@ combine ())


let doc = "$(iname) - clear the console screen"

let man = [
  `S Manpage.s_description;
  `P "Clear the console screen"
]

let navigate phone run = 
  let info = 
    Cmd.info ~doc ~man  name
  in
  Cmd.v info (cmd_term phone run)

let run _ cmd = 
  let () = cmd in
  let () = Printf.printf "%s" clear_console in
  Ok ()

let command phone = navigate phone run


let eval argv phone = Cmd.eval_result ~argv (command phone)