extern "C" {
#include <iwasm/include/wasm_c_api.h>
#include <iwasm/include/wasm_export.h>
#include <iwasm/include/lib_export.h>
#include <iwasm/interpreter/wasm_runtime.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
}

#include <fstream>
#include <iostream>

/* the native functions that will be exported to WASM app */
static NativeSymbol native_symbols[] = {
    // EXPORT_WASM_API_WITH_SIG(display_input_read, "(*)i"),
    // EXPORT_WASM_API_WITH_SIG(display_flush, "(iiii*)")
};

/* all the runtime memory allocations are retricted in the global_heap_buf array */
static char global_heap_buf[512 * 1024];

void read_wasm_binary_to_buffer(char* path, std::string &buffer) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    if (!file.is_open()) {
        perror(path);
    }

    file >> buffer;
}

extern "C" char* read_wasm_binary_to_buffer_c(char* path, uint32_t* size) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        perror(path);
        return NULL;
    }

    struct stat st;
    int serr = stat(path, &st);
    if (serr == -1) {
        perror("stat");
        return NULL;
    }
    *size = (uint32_t) st.st_size;

    char* buffer = (char *) malloc(*size);
    fread(buffer, *size, 1, file);

    return buffer;
}

extern "C" void load_wasm() {
    printf("load wasm\n");
    char error_buf[128];
    std::string buffer;
    wasm_module_t module;
    wasm_module_inst_t module_inst;
    wasm_function_inst_t func;
    wasm_exec_env_t exec_env;
    uint32_t stack_size = 8092*8092, heap_size = 8092 * 8;

    /* initialize the wasm runtime by default configurations */
    wasm_runtime_init();

    RuntimeInitArgs init_args;
    memset(&init_args, 0, sizeof(RuntimeInitArgs));

    /* configure the memory allocator for the runtime */
    init_args.mem_alloc_type = Alloc_With_Pool;
    init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
    init_args.mem_alloc_option.pool.heap_size = sizeof(global_heap_buf);

    /* set maximum thread number if needed when multi-thread is enabled,
    the default value is 4 */
    init_args.max_thread_num = 4;

    /* initialize runtime environment with user configurations*/
    if (!wasm_runtime_full_init(&init_args)) {
        exit(-1);
    }

    int n_native_symbols = sizeof(native_symbols) / sizeof(NativeSymbol);
    if (!wasm_runtime_register_natives("env", native_symbols, n_native_symbols)) {
        exit(-1);
    }

    /* read WASM file into a memory buffer */
    read_wasm_binary_to_buffer("test.wasm", buffer);
    if (buffer.empty()) {
        exit(-1);
    }

    uint32_t size;
    char *bin = read_wasm_binary_to_buffer_c("test.wasm", &size);

    printf("cpp:%d vs c:%d\n", buffer.size(), size);

    /* parse the WASM file from buffer and create a WASM module */
    module = wasm_runtime_load((uint8_t *) bin, size, error_buf, sizeof(error_buf));

    if (module == NULL) {
        printf("%s\n", error_buf);
        exit(-1);
    }

    /* create an instance of the WASM module (WASM linear memory is ready) */
    module_inst = wasm_runtime_instantiate(module, stack_size, heap_size, error_buf, sizeof(error_buf));
    if (module_inst == NULL) {
        printf("%s\n", error_buf);
        exit(-1);
    }

    /* lookup a WASM function by its name
     The function signature can NULL here */

    /* creat an execution environment to execute the WASM functions */
    exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);

    func = wasm_runtime_lookup_function(module_inst, "fib");
    if (func == NULL) {
        printf("error to find the function\n");
        exit(-1);
    }
    uint32_t argv[2];

    /* arguments are always transferred in 32-bit element */
    argv[0] = 8;
    printf("run fib function\n");
    /* call the WASM function */
    if (wasm_runtime_call_wasm(exec_env, func, 1, argv)) {
        /* the return value is stored in argv[0] */
        printf("fib function return: %d\n", argv[0]);
    } else {
        /* exception is thrown if call fails */
        wasm_runtime_dump_call_stack(exec_env);
        printf("%s\n", wasm_runtime_get_exception(module_inst));
    }
    exit(0);
}