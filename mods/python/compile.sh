cargo build --target wasm32-unknown-unknown --lib --release
rm ../../build/test.wasm
cp target/wasm32-unknown-unknown/release/rust.wasm ../../build/test.wasm