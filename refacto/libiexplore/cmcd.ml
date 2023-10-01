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

let name = "cd"

let clear_console = "\u{001B}[2J"

type cmd = {
  path : string option
}


let path_term = 
  Arg.(
    value & pos 0 (some string) None & info [] ~doc:"directory" ~docv:"directory"
  )

let cmd_term phone run = 
  let combine path = run phone {path}
  in
  Term.(const @@ combine $ path_term)


let doc = "$(iname) - change working directory"

let man = [
  `S Manpage.s_description;
  `P "Change working directory"
]

let navigate phone run = 
  let info = 
    Cmd.info ~doc ~man  name
  in
  Cmd.v info (cmd_term phone run)

let is_absolute = String.starts_with ~prefix:"/"

let run (phone : Phone.t) cmd = 
  let {path} = cmd in
  let (let*) = Result.bind in
  let ( |? ) f m = Result.map_error m f in
  match path with
    | None -> let () = phone.path <- Path.root in Ok ()
    | Some path -> 
      let path = match is_absolute path with
        | true -> Path.of_string @@ String.sub path (1 ) (String.length path - 1)
        | false -> let next = Path.of_string path in Path.concat next phone.path 
      in
      let* _ = 
        Cbindings.ImobileDevice.afc_read_directory phone.client @@ Path.to_string path 
          |? (fun e -> Printexc.to_string @@ Error.exn_explore_error @@ Error.afc_error e) 
      in 
      let () = phone.path <- path in
      Ok ()

let command phone = navigate phone run


let eval argv phone = Cmd.eval_result ~argv (command phone)