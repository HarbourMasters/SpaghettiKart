GOOS=wasip1 GOARCH=wasm tinygo build -o testgo.wasm test.go
rm ../../build/test.wasm
cp testgo.wasm ../../build/test.wasm