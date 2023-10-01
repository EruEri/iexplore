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

type t = string list

let (//) = Printf.sprintf "%s/%s"

let rec rev_concat = function 
  | []  -> "/"
  | t::[] -> Printf.sprintf "/%s" t
  | t::q -> 
    let head = rev_concat q in
    head // t

let to_string = rev_concat

let of_string s =
  s |> String.split_on_char '/' |> List.rev

let root = []

let backward = function
  | [] -> [] 
  | _::q -> q

let push elt path = match elt with
    | "." -> path
    | ".." -> backward path
    | elt -> elt::path


let concat base next = 
  next @ base