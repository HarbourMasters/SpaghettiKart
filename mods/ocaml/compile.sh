ocamlfind ocamlc -package js_of_ocaml,js_of_ocaml-ppx,js_of_ocaml-lwt -linkpkg -o test.byte test.mli test.ml
wasm_of_ocaml test.byte