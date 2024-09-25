py2many . --cpp=1 --outdir out
echo "#include \"../export.cpp\"" >> out/main.cpp
emcc out/main.cpp -o out/testpython.wasm -O3 -s LINKABLE=1 -s EXPORT_ALL=1 -s PURE_WASI=1
rm ../../build/test.wasm
cp out/testpython.wasm ../../build/test.wasm
