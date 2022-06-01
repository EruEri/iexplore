let rec get_input message () = 
  print_string message;
  match read_line () with
  | s when s |> int_of_string_opt |> Option.is_some -> `Number (int_of_string s)
  | "i" | "info" -> `Info
  | "q" | "quit" -> `Quit
  | "c" | "copy" -> `Copy
  | _ -> Printf.printf "Wrong option\n"; get_input message ()
;;

let rec read_int_safe message error_message () = 
  (print_string message);
  match read_int_opt ()with
  | Some n -> n
  | None -> print_string error_message; read_int_safe message error_message()
;;

let read_line_message () = 
  Printf.printf "Output path\n> ";
  read_line ()

let () = Callback.register "get_input" get_input;;
let () = Callback.register "read_int" read_int_safe;;
let () = Callback.register "read_line" read_line_message;;