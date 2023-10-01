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

let name = "iexplore"


(* let version =
  let s =
    match Build_info.V1.version () with
    | None ->
        "n/a"
    | Some v ->
        Build_info.V1.Version.to_string v
  in
  Printf.sprintf "%s" s *)

let root_doc = "an iphone file explorer"

let root_man =
  [
    `S Manpage.s_description;
    `P "$(iname) allows to browser file within your iPhone";
  ]

let root_info = Cmd.info name ~doc:root_doc ~man:root_man ~version:"0.2.0"
let subcommands = [ Navigate.command ]
let parse () = Cmd.group root_info subcommands

let eval () =
  () |> parse |> Cmd.eval

