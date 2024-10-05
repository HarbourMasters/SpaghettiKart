#include <stdio.h>
#include <stdlib.h>

__attribute__((import_name("call_extern_function"))) int call_extern_function(char* module, char* function_name,
                                                                              int argc, int* argv);

int fib(int n) {
    call_extern_function("test", "testfunc", 0, NULL);
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