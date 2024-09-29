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
#include <vector>
#include <filesystem>

struct mod_instance {
    wasm_exec_env_t exec_env;
    char* name;
};

std::vector<mod_instance> mods_instance = {};

void load_mod_wasm_file(char* mod_name, char* buffer, size_t size) {
    char error_buf[128];
    wasm_module_t module;
    wasm_module_inst_t module_inst;
    wasm_exec_env_t exec_env;
    uint32_t stack_size = 8092 * 8092, heap_size = 8092 * 8;
    /* parse the WASM file from buffer and create a WASM module */
    module = wasm_runtime_load((uint8_t*) buffer, size, error_buf, sizeof(error_buf));
    printf("error? %s\n", error_buf);

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
    mods_instance.push_back({ exec_env, mod_name });
}

mod_instance* find_mod(char* name) {
    for (int i = 0; i < mods_instance.size(); i++) {
        if (strcmp(mods_instance[i].name, name) == 0) {
            return &mods_instance[i];
        }
    }
    return NULL;
}

wasm_module_inst_t get_mods_instance(char* mod_name) {
    return wasm_exec_env_get_module_inst(find_mod(mod_name)->exec_env);
}

wasm_function_inst_t mod_lookup_function(char* mod_name, char* function_name) {
    return wasm_runtime_lookup_function(get_mods_instance(mod_name), function_name);
}

bool mod_call_wasm(char* mod_name, wasm_function_inst_t function, uint32 argc, uint32 argv[]) {
    return wasm_runtime_call_wasm(find_mod(mod_name)->exec_env, function, 1, argv);
}

void mod_print_exception(char* mod_name) {
    printf("%s\n", wasm_runtime_get_exception(get_mods_instance(mod_name)));
}

bool mod_execute_function(wasm_exec_env_t exec_env, uint32 msg_offset, uint32 buffer_offset, int32 buf_len) {
    wasm_module_inst_t module_inst = get_module_inst(exec_env);
    char* buffer;
    char* msg;

    // do boundary check
    if (!wasm_runtime_validate_app_str_add(msg_offset))
        return 0;

    if (!wasm_runtime_validate_app_addr((uint64) buffer_offset, (uint64) buf_len))
        return;

    // do address conversion
    buffer = wasm_runtime_addr_app_to_native((uint64) buffer_offset);
    msg = wasm_runtime_addr_app_to_native((uint64) msg_offset);

    strncpy(buffer, msg, buf_len);
}

/* the native functions that will be exported to WASM app */
static NativeSymbol native_symbols[] = {
    // EXPORT_WASM_API_WITH_SIG(display_input_read, "(*)i"),
    // EXPORT_WASM_API_WITH_SIG(display_flush, "(iiii*)")
};

/* all the runtime memory allocations are retricted in the global_heap_buf array */
static char global_heap_buf[512 * 1024];

char* read_wasm_binary_to_buffer(char* path, uint32_t* size) {
    std::ifstream file;
    file.open(path, std::ios::binary);
    if (!file.is_open()) {
        perror(path);
        return NULL;
    }

    file.seekg(0, std::ios::end);

    *size = file.tellg();

    file.seekg(0);

    char* c_buffer = (char*) malloc(*size);
    file.read(c_buffer, *size);
    return c_buffer;
}

extern "C" void load_wasm() {
    printf("load wasm\n");
    char* buffer;
    wasm_function_inst_t func;

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
    uint32_t size;
    /* read WASM file into a memory buffer */
    buffer = read_wasm_binary_to_buffer("test.wasm", &size);
    if (buffer == NULL) {
        printf("error read binary\n");
        exit(-1);
    }

    load_mod_wasm_file("test", buffer, size);

    func = mod_lookup_function("test", "fib");
    if (func == NULL) {
        printf("error to find the function\n");
        exit(-1);
    }
    uint32_t argv[2];

    /* arguments are always transferred in 32-bit element */
    argv[0] = 8;
    printf("run fib function\n");
    /* call the WASM function */
    if (mod_call_wasm("test", func, 1, argv)) {
        /* the return value is stored in argv[0] */
        printf("fib function return: %d\n", argv[0]);
    } else {
        /* exception is thrown if call fails */
        // wasm_runtime_dump_call_stack(exec_env);
        mod_print_exception("test");
    }
    exit(0);
}