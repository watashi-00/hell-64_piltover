//
// Created by watashi on 9/25/26.
//

#include "vm.h"
#include "bytecode.h"

static int valid_register(uint8_t reg) {
    return reg < 16;
}

static int validate_instruction(const VmInstruction *instruction) {
    switch (instruction->opcode) {
        case VM_OP_MOV:
            return valid_register(instruction->dst) &&
                   valid_register(instruction->src1);

        case VM_OP_MOVI:
            return valid_register(instruction->dst);

        case VM_OP_ADD:
        case VM_OP_SUB:
        case VM_OP_MUL:
        case VM_OP_XOR:
            return valid_register(instruction->dst) &&
                   valid_register(instruction->src1) &&
                   valid_register(instruction->src2);

        case VM_OP_LOAD:
            return valid_register(instruction->dst) &&
                   valid_register(instruction->src1);

        case VM_OP_STORE:
            return valid_register(instruction->src1) &&
                   valid_register(instruction->src2);

        default:
            return 0;
    }
}

int vm_validate_bytecode(const Vm *vm) {
    if (vm == 0)
        return 0;

    if (vm->bytecode == 0)
        return 0;

    if (vm->bytecode_size == 0)
        return 0;

    if (vm->bytecode_size % sizeof(VmInstruction) != 0)
        return 0;

    uint64_t instruction_count =
            vm->bytecode_size / sizeof(VmInstruction);

    for (uint64_t i = 0; i < instruction_count; i++) {
        VmInstruction *instruction =
                (VmInstruction *) (vm->bytecode +
                                   i * sizeof(VmInstruction));

        if (!validate_instruction(instruction))
            return 0;
    }

    return 1;
}
