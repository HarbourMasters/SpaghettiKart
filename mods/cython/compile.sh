python setup.py build_ext --inplace
rm ../../build/test.wasm
cp test.cp311-wasm32_wasip1_threads.wasm ../../build/test.wasm