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


(*
  Maybe in a config file one day   
*)
let prompt = "> "

let read () =
  let () = Printf.printf "%s" prompt in
  let s = read_line () in
  s 

module Commands = Map.Make(String)

let commands = 
  Commands.empty
  |> Commands.add Cmls.name ( module Cmls : Command.Command)
  |> Commands.add Cmclear.name (module Cmclear : Command.Command)

let rec repl code phone = 
  try 
    let contents = read () in
    let contents = String.split_on_char ' '  contents in
    match contents with
      | [] -> repl 0 phone
      | "exit"::_ -> ()
      | "$?"::_ -> let () = Printf.printf "%u" code in repl 0 phone
      | ( t::_ as argv )-> 
        let (module C) = Commands.find t commands in
        let i = C.eval (Array.of_list argv) phone in
        repl i phone
  with 
    End_of_file -> () 
    | Not_found -> let () = prerr_endline "unknwon command" in repl 127 phone

let repl = repl 0