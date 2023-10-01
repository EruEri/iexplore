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

module type Command = sig
  val name : string

  val eval : string array -> Phone.t -> int
end