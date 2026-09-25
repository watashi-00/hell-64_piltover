#include "native_functions.h"


void runtime_start(void *stack)
{
    (void)stack;
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
    println("\n");
}
