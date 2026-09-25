//
// Created by watashi on 9/25/26.
//

#ifndef HELL_64_PILTOVER_VM_H
#define HELL_64_PILTOVER_VM_H

#include <stdint.h>

typedef struct vm Vm;
typedef struct vm_task VmTask;

struct vm {
    uint8_t *bytecode;
    uint64_t bytecode_size;

    uint8_t *shared_mem;
    uint64_t shared_mem_size;
};

struct vm_task {
    Vm *vm;

    uint64_t registers[16];
    uint64_t pc;

    uint8_t *stack;
    uint64_t stack_size;
};

typedef enum {
    OP_MOV,
    OP_MOVI,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_XOR,
    OP_LOAD,
    OP_STORE,
    OP_JMP,
    OP_JZ,
    OP_CALL,
    OP_RET,
    OP_ATOMIC_ADD,
    OP_CAS,
    OP_SPAWN,
    OP_JOIN,
    OP_YIELD,
    OP_HALT
} Opcode;

Vm *vm_create(uint64_t shared_mem_size);

void vm_destroy(Vm *vm);

VmTask *vm_task_create(Vm *vm, uint64_t stack_size);

void vm_task_destroy(VmTask *vm_task);

#endif //HELL_64_PILTOVER_VM_H
