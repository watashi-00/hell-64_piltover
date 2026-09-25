#include "native_functions.h"
#include "allocator.h"

void runtime_start(void *stack) {
    (void) stack;

    char *memory = vm_alloc(4096);

    if (memory == 0) {
        println("memory allocation failed");
        return;
    }

    const char msg[] = "write potato: ";
    println(msg);

    char buf[128];
    scan(buf);

    char p[] = "potato";

    bool e = string_equals(p, buf);

    if (e) {
        println("you wrote potato\n");
    } else {
        println("you didn't wrote potato\n");
    }

    println(buf);
}
