let rec get_input message () = 
  print_string message;
  match read_line () with
  | s when s |> int_of_string_opt |> Option.is_some -> `Number (int_of_string s)
  | "i" | "info" -> `Info
  | "q" | "quit" -> `Quit
  | "c" | "copy" -> `Copy
  | _ -> Printf.printf "Wrong option\n"; get_input message ()
;;

let () = Callback.register "get_input" get_input;;