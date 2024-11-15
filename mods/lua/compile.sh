sudo docker pull ysugimoto/webassembly-lua
sudo docker run --rm -v $PWD:/src ysugimoto/webassembly-lua emcc-lua
rm ../../build/test.wasm
cp testlua.wasm ../../build/test.wasm