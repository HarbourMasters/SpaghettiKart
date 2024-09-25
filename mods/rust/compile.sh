cargo build --target wasm32-wasip1 --release
rm ../../build/test.wasm
cp target/wasm32-wasip1/release/rust.wasm ../../build/test.wasm