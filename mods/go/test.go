package main

//export fib
func fib(n uint32) uint32 {
	if (n <= 1) {
        return n
    }
    return fib(n - 1) + fib(n - 2)
}

// main is required for the `wasip1` target, even if it isn't used.
func main() {}
