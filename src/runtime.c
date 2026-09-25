#include "native_functions.h"
#include "allocator.h"
#include "vm.h"

void runtime_start(void *stack) {
    (void) stack;

    Vm *vm = vm_create(4096);

    if (vm == 0) {
        println("VM Creation failed");
        return;
    }

    VmTask *task = vm_task_create(vm, 4096);

    if (task == 0) {
        println("Task Creation failed");
        vm_destroy(vm);
        return;
    }

    task->registers[0] = 42;

    if (task->registers[0] == 42) {
        println("vm lifecycle ok");
    }

    vm_task_destroy(task);
    vm_destroy(vm);

}
