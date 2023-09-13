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

module C = Configurator.V1

let cflag = Printf.sprintf "cflags_%s.sexp"
let clibs = Printf.sprintf "clibrary_flags_%s.sexp"
let generate_flag_libs name = (cflag name, clibs name)

let pkg_config libname () =
  let cflags, clibs = generate_flag_libs libname in
  C.main ~name:libname (fun c ->
      let conf =
        match C.Pkg_config.get c with
        | None ->
            C.die "pkg-config not found"
        | Some pc -> (
            match C.Pkg_config.query pc ~package:libname with
            | None ->
                C.die "%s pkg-config query not found" libname
            | Some deps ->
                deps
          )
      in
      C.Flags.write_sexp cflags conf.cflags;
      C.Flags.write_sexp clibs conf.libs
  )

let () = pkg_config "libimobiledevice" ()