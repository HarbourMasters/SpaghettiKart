#include <stdio.h>
#include <stdlib.h>

#define IMPORT_FUNC(return_type, name) __attribute__((import_name(#name))) return_type name

IMPORT_FUNC(int, call_extern_function)(char* module, char* function_name, int argc, int* argv);
IMPORT_FUNC(void, hook_render)(void());
IMPORT_FUNC(void, load_debug_font)();
IMPORT_FUNC(void, post_debug_print)();
IMPORT_FUNC(void, debug_print_str2)(int x, int y, char*);

int fib(int n) {
    // call_extern_function("test", "testfunc", 0, NULL);
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

void testfunc() {
    printf("call testfunc with the function test\n");
}

void some_render() {
    load_debug_font();
    debug_print_str2(0, 0, "hello");
    post_debug_print();
}

void init() {
    printf("init test\n");
    hook_render(some_render);
}

int main() {
}