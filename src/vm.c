//
// Created by watashi on 9/25/26.
//

#include "vm.h"
#include "allocator.h"
#include "validator.h"

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
    for (uint64_t i = 0; i < VM_MAX_TASKS; i++)
        atomic_init(&vm->task_table[i], 0);
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
    task->call_depth = 0;

    for (uint64_t slot = 0; slot < VM_MAX_TASKS; slot++) {
        VmTask *empty = 0;
        if (atomic_compare_exchange_strong(&vm->task_table[slot], &empty, task)) {
            task->task_id = slot + 1;
            return task;
        }
    }

    vm_free_n(task->stack, task->stack_size);
    vm_free_n(task, sizeof(VmTask));
    return 0;
}

void vm_task_destroy(VmTask *task) {
    if (task == 0)
        return;

    if (task->vm != 0 && task->task_id > 0 && task->task_id <= VM_MAX_TASKS)
        atomic_compare_exchange_strong(&task->vm->task_table[task->task_id - 1],
                                       &task, 0);

    if (task->stack != 0)
        vm_free_n(task->stack, task->stack_size);

    vm_free_n(task, sizeof(VmTask));
}

VmTask *vm_task_lookup(Vm *vm, uint64_t task_id) {
    if (vm == 0 || task_id == 0 || task_id > VM_MAX_TASKS)
        return 0;
    return atomic_load(&vm->task_table[task_id - 1]);
}

static uint64_t *shared_word(Vm *vm, uint64_t base, uint64_t displacement) {
    if (vm == 0 || vm->shared_mem == 0 || vm->shared_mem_size < sizeof(uint64_t) ||
        base > vm->shared_mem_size - sizeof(uint64_t) ||
        displacement > vm->shared_mem_size - sizeof(uint64_t) - base)
        return 0;

    return (uint64_t *) (vm->shared_mem + base + displacement);
}

int vm_execute(VmTask *task) {
    Vm *vm;
    uint64_t instruction_count;

    if (task == 0 || task->vm == 0)
        return 0;

    vm = task->vm;
    if (!vm_validate_bytecode(vm))
        return 0;

    instruction_count = vm->bytecode_size / sizeof(VmInstruction);

    while (task->pc < instruction_count) {
        VmInstruction *instruction =
                (VmInstruction *) (vm->bytecode +
                                   task->pc * sizeof(VmInstruction));

        switch (instruction->opcode) {
            case VM_OP_MOV:
                task->registers[instruction->dst] =
                        task->registers[instruction->src1];
                task->pc++;
                break;
            case VM_OP_MOVI:
                task->registers[instruction->dst] = instruction->operand;
                task->pc++;
                break;
            case VM_OP_ADD:
                task->registers[instruction->dst] =
                        task->registers[instruction->src1] +
                        task->registers[instruction->src2];
                task->pc++;
                break;
            case VM_OP_SUB:
                task->registers[instruction->dst] =
                        task->registers[instruction->src1] -
                        task->registers[instruction->src2];
                task->pc++;
                break;
            case VM_OP_MUL:
                task->registers[instruction->dst] =
                        task->registers[instruction->src1] *
                        task->registers[instruction->src2];
                task->pc++;
                break;
            case VM_OP_XOR:
                task->registers[instruction->dst] =
                        task->registers[instruction->src1] ^
                        task->registers[instruction->src2];
                task->pc++;
                break;
            case VM_OP_LOAD: {
                uint64_t *address = shared_word(vm,
                                                task->registers[instruction->src1],
                                                instruction->operand);
                if (address == 0)
                    return 0;
                task->registers[instruction->dst] = *address;
                task->pc++;
                break;
            }
            case VM_OP_STORE: {
                uint64_t *address = shared_word(vm,
                                                task->registers[instruction->src1],
                                                instruction->operand);
                if (address == 0)
                    return 0;
                *address = task->registers[instruction->src2];
                task->pc++;
                break;
            }
            case VM_OP_ATOMIC_ADD: {
                uint64_t *address = shared_word(vm, task->registers[instruction->src1], 0);
                if (address == 0)
                    return 0;
                __atomic_fetch_add(address, task->registers[instruction->src2],
                                   __ATOMIC_SEQ_CST);
                task->pc++;
                break;
            }
            case VM_OP_CAS: {
                uint64_t *address = shared_word(vm, task->registers[instruction->src1], 0);
                uint64_t expected;
                if (address == 0)
                    return 0;
                expected = task->registers[instruction->src2];
                __atomic_compare_exchange_n(address, &expected, instruction->operand,
                                             0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
                task->registers[instruction->dst] = expected;
                task->pc++;
                break;
            }
            case VM_OP_HALT:
                task->pc++;
                return 1;
            case VM_OP_JMP:
                task->pc = instruction->operand;
                break;
            case VM_OP_JZ:
                if (task->registers[instruction->src1] == 0)
                    task->pc = instruction->operand;
                else
                    task->pc++;
                break;
            case VM_OP_CALL:
                if (task->stack == 0 || task->stack_size < sizeof(uint64_t) ||
                    task->stack_size / sizeof(uint64_t) <= task->call_depth)
                    return 0;
                ((uint64_t *) task->stack)[task->call_depth++] = task->pc + 1;
                task->pc = instruction->operand;
                break;
            case VM_OP_RET:
                if (task->call_depth == 0)
                    return 0;
                task->pc = ((uint64_t *) task->stack)[--task->call_depth];
                break;
            default:
                /* Unsupported instructions return safely after a checked fetch. */
                task->pc++;
                return 0;
        }
    }

    return 0;
}
