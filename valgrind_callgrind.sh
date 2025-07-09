timeout 2m valgrind --tool=callgrind --callgrind-out-file=spaghetti.out ./build/Spaghettify
callgrind_annotate spaghetti.out > spaghetti_bench.out