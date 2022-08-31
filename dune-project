(lang dune 3.1)

(name iexplore)

(generate_opam_files true)

(source
 (github EruEri/iexplore))

(authors "EruEri")

(maintainers "EruEri")

(license GPL-3.0)


(package
 (name iexplore)
 (synopsis "Little program to explore Iphone's internal files")
 (depends ocaml dune clap)
 (tags
  (iPhone)))

; See the complete stanza docs at https://dune.readthedocs.io/en/stable/dune-files.html#dune-project
