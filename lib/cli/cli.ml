open Caml_core

let main_start () = 

  Clap.description "A litle utiliry to navigate into your iPhone";

  let command = Clap.subcommand [
    (
      
      Clap.case ~description:"Navigate througth the iPhone" "navigate" ( fun () -> 
      `navigate
      )
    );
    (
      Clap.case ~description:"Download the music" "download-music" ( fun () -> 

        let output_dir = Clap.optional_string ~long: "output" ~short: 'o' ~description: "Output directory" () in
        let all = Clap.flag ~set_long: "all" ~description: "Used to downlaod all the tracks" false in
        let limit = Clap.optional_int ~long: "limit" ~short:'l' ~description: "Set number of tracks that will be copied" () in
        let offset = Clap.optional_int ~long: "offset" ~description: "Number of tracks skipped before copying" () in
        `dl_music (output_dir, all, limit, offset)
      )
    )
  ]
  in
   
  Clap.close ();

  let connection = match new_usb_device () with
  | Error e ->  failwith (Printf.sprintf "Error : %s\n" (string_of_idevice_error e)); 
  | Ok c -> c in


  match command with
  | `navigate -> Caml_core.navigate_device connection ()
  | `dl_music (output_dir, all, limit, offset) -> 
    output_dir |> Option.iter (Printf.printf "ouput_dir = %s\n");
    Printf.printf "all = %b" (all);
    limit |> Option.iter (Printf.printf "limit = %d\n");
    offset |> Option.iter (Printf.printf "offset = %d\n");
()