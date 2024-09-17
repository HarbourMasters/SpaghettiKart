~/Documents/bin/javy-x86_64-linux-v3.1.0 build mods/js/test.js -C wit=mods/js/test.wit -C wit-world=index-world -o testjs.wasm
rm build/test.wasm
cp testjs.wasm build/test.wasm