extern "C" {
__attribute__((export_name("fib"))) int exp_fib(int n) {
    return fib(n);
}
}
