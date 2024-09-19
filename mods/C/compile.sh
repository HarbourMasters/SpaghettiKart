emcc test.c -o testc.wasm -O3 -s LINKABLE=1 -s EXPORT_ALL=1 -s PURE_WASI=1
rm ../../build/test.wasm
cp testc.wasm ../../build/test.wasm