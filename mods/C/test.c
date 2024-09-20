#include <stdio.h>

extern __attribute__((import_module("testmodule"), import_name("testfunc"))) void test();

int fib(int n) {
    test();
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

void testfunc() {
    printf("call testfunc with the function test\n");
}

int main() {
}