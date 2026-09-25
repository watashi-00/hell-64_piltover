#include "native_functions.h"

void runtime_start(void *stack)
{
    (void)stack;
    const char msg[] = "runtime_start called!";
    println(msg);
}
