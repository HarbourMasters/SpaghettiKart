use std::env;

use rustpython::vm;

fn main() {
    let mut setting: vm::Settings = Default::default();
    setting.path_list.push("".to_string());
    let inter = vm::Interpreter::without_stdlib(setting);
    let ret: vm::PyResult<()> = inter.enter(|vm| {
        let scope = vm.new_scope_with_builtins();
        let source = r#"print("Hello World from python!")"#;
        let code_obj = vm
            .compile(source, vm::compiler::Mode::Exec, "<embedded>".to_owned())
            .map_err(|err| vm.new_syntax_error(&err, Some(source)))?;

        vm.unwrap_pyresult(vm.run_code_obj(code_obj, scope));

        Ok(())
    });
}

#[no_mangle]
pub fn fib(n: u32) -> u32 {
    if n <= 1 {
        main();
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}
