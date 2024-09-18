use wasm_bindgen::prelude::*;
use rustpython::vm;

fn main() {
    rustpython::run(|vm| {
        let scope = vm.new_scope_with_builtins();
        let source = r#"print("Hello World!")"#;
        let code_obj = vm
            .compile(source, vm::compiler::Mode::Exec, "<embedded>".to_owned())
            .map_err(|err| vm.new_syntax_error(&err, Some(source))).unwrap();

        vm.run_code_obj(code_obj, scope);
    });
}

#[wasm_bindgen]
pub fn fib(n: u32) -> u32 {
    if n <= 1 {
        main();
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}
