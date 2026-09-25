//
// Created by watashi on 9/25/26.
//

#ifndef HELL_64_PILTOVER_BYTECODE_H
#define HELL_64_PILTOVER_BYTECODE_H
#include <stdint.h>

typedef enum {
    VM_OP_MOV = 0,
    VM_OP_MOVI,
    VM_OP_ADD,
    VM_OP_SUB,
    VM_OP_MUL,
    VM_OP_XOR,

    VM_OP_LOAD,
    VM_OP_STORE,

    VM_OP_JMP,
    VM_OP_JZ,

    VM_OP_CALL,
    VM_OP_RET,

    VM_OP_ATOMIC_ADD,
    VM_OP_CAS,

    VM_OP_SPAWN,
    VM_OP_JOIN,
    VM_OP_YIELD,

    VM_OP_HALT
} VmOpcode;

typedef struct {
    uint8_t opcode;
    uint8_t dst;
    uint8_t src1;
    uint8_t src2;
    uint64_t operand;
} VmInstruction;

_Static_assert(sizeof(VmInstruction) == 16,
               "VmInstruction must be 16 bytes");

#endif //HELL_64_PILTOVER_BYTECODE_H
