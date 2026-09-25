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
