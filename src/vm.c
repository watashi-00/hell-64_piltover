//
// Created by watashi on 9/25/26.
//

#include "vm.h"
#include "allocator.h"

Vm *vm_create(uint64_t shared_mem_size) {
    Vm *vm = vm_alloc(sizeof(Vm));

    if (vm == 0)
        return 0;

    vm->bytecode = 0;
    vm->bytecode_size = 0;

    vm->shared_mem = vm_alloc(shared_mem_size);

    if (vm->shared_mem == 0) {
        vm_free_n(vm, sizeof(Vm));
        return 0;
    }

    vm->shared_mem_size = shared_mem_size;
    return vm;
}

void vm_destroy(Vm *vm) {
    if (vm == 0)
        return;

    if (vm->bytecode != 0)
        vm_free_n(vm->bytecode, vm->bytecode_size);

    if (vm->shared_mem != 0)
        vm_free_n(vm->shared_mem, vm->shared_mem_size);

    vm_free_n(vm, sizeof(Vm));
}

VmTask *vm_task_create(Vm *vm, uint64_t stack_size) {
    if (vm == 0)
        return 0;

    VmTask *task = vm_alloc(sizeof(VmTask));

    if (task == 0)
        return 0;

    task->vm = vm;

    for (int i = 0; i < 16; i++)
        task->registers[i] = 0;

    task->pc = 0;

    task->stack = vm_alloc(stack_size);

    if (task->stack == 0) {
        vm_free_n(task, sizeof(VmTask));
        return 0;
    }

    task->stack_size = stack_size;

    return task;
}

void vm_task_destroy(VmTask *task) {
    if (task == 0)
        return;

    if (task->stack != 0)
        vm_free_n(task->stack, task->stack_size);

    vm_free_n(task, sizeof(VmTask));
}
