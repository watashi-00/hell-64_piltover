//
// Created by watashi on 9/25/26.
//

#ifndef HELL_64_PILTOVER_VM_H
#define HELL_64_PILTOVER_VM_H

#include <stdint.h>
#include <stdatomic.h>
#include "bytecode.h"

typedef struct vm Vm;
typedef struct vm_task VmTask;

#define VM_MAX_TASKS 1024

struct vm {
    uint8_t *bytecode;
    uint64_t bytecode_size;

    uint8_t *shared_mem;
    uint64_t shared_mem_size;

    _Atomic(VmTask *) task_table[VM_MAX_TASKS];
};

struct vm_task {
    Vm *vm;

    uint64_t registers[16];
    uint64_t pc;

    uint8_t *stack;
    uint64_t stack_size;
    uint64_t call_depth;
    uint64_t task_id;
};

Vm *vm_create(uint64_t shared_mem_size);

void vm_destroy(Vm *vm);

VmTask *vm_task_create(Vm *vm, uint64_t stack_size);

void vm_task_destroy(VmTask *vm_task);

VmTask *vm_task_lookup(Vm *vm, uint64_t task_id);

int vm_execute(VmTask *task);

#endif //HELL_64_PILTOVER_VM_H
