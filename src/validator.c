//
// Created by watashi on 9/25/26.
//

#include "vm.h"
#include "bytecode.h"

int vm_validate_bytecode(const Vm *vm) {
    if (vm == 0)
        return 0;

    if (vm->bytecode == 0)
        return 0;

    if (vm->bytecode_size == 0)
        return 0;

    if (vm->bytecode_size % sizeof(VmInstruction) != 0)
        return 0;

    return 1;
}