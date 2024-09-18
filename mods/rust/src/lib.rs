mod utils;

use wasm_bindgen::prelude::*;

#[wasm_bindgen]
pub fn fib(n: u32) -> u32 {
    if n <= 1 {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}
