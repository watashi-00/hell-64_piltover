#include "test_harness.h"
#include "vm.h"
#include "allocator.h"

void test_vm_create_and_destroy(void) {
    Vm *vm = vm_create(4096);
    ASSERT_NOT_NULL(vm, "vm_create(4096) failed");
    ASSERT_EQ(vm->shared_mem_size, 4096, "vm shared_mem_size mismatch");
    ASSERT_NOT_NULL(vm->shared_mem, "vm shared_mem is NULL");

    vm_destroy(vm);
}

void test_vm_task_create_and_destroy(void) {
    Vm *vm = vm_create(4096);
    ASSERT_NOT_NULL(vm, "vm_create failed for task test");

    VmTask *task = vm_task_create(vm, 2048);
    ASSERT_NOT_NULL(task, "vm_task_create(vm, 2048) failed");
    ASSERT_EQ(task->vm, vm, "task vm pointer mismatch");
    ASSERT_EQ(task->task_id, 1, "task ID mismatch");
    ASSERT_EQ(vm_task_lookup(vm, task->task_id), task, "task lookup failed");
    ASSERT_EQ(task->stack_size, 2048, "task stack_size mismatch");
    ASSERT_NOT_NULL(task->stack, "task stack is NULL");

    task->registers[0] = 42;
    ASSERT_EQ(task->registers[0], 42, "register value failed");

    vm_task_destroy(task);
    ASSERT_EQ(vm_task_lookup(vm, 1), 0, "destroyed task remained in table");
    vm_destroy(vm);
}

void test_vm_lifecycle(void) {
    Vm *vm = vm_create(4096);
    ASSERT_NOT_NULL(vm, "VM creation failed in lifecycle test");

    VmTask *task = vm_task_create(vm, 4096);
    ASSERT_NOT_NULL(task, "Task creation failed in lifecycle test");

    task->registers[0] = 42;
    ASSERT_EQ(task->registers[0], 42, "VM task register check failed");

    vm_task_destroy(task);
    vm_destroy(vm);
}

void test_vm_execute_halt(void) {
    Vm *vm = vm_create(4096);
    VmTask *task;
    VmInstruction *instructions;

    ASSERT_NOT_NULL(vm, "VM creation failed in execute test");
    vm->bytecode_size = 2 * sizeof(VmInstruction);
    vm->bytecode = vm_alloc(vm->bytecode_size);
    ASSERT_NOT_NULL(vm->bytecode, "Bytecode allocation failed");
    instructions = (VmInstruction *) vm->bytecode;
    instructions[0].opcode = VM_OP_YIELD;
    instructions[1].opcode = VM_OP_HALT;

    task = vm_task_create(vm, 1024);
    ASSERT_NOT_NULL(task, "Task creation failed in execute test");
    ASSERT_EQ(vm_execute(task), 0, "Unsupported instruction was not rejected");
    ASSERT_EQ(task->pc, 1, "PC did not advance after checked fallback fetch");
    task->pc = 1;
    ASSERT_EQ(vm_execute(task), 1, "HALT did not terminate execution");
    ASSERT_EQ(task->pc, 2, "PC did not advance over HALT");

    vm_task_destroy(task);
    vm_destroy(vm);
}

void test_vm_execute_rejects_invalid_bytecode(void) {
    Vm *vm = vm_create(4096);
    VmTask *task;

    ASSERT_NOT_NULL(vm, "VM creation failed in invalid bytecode test");
    task = vm_task_create(vm, 1024);
    ASSERT_NOT_NULL(task, "Task creation failed in invalid bytecode test");
    ASSERT_EQ(vm_execute(task), 0, "Invalid bytecode was not rejected");
    ASSERT_EQ(task->pc, 0, "PC changed for invalid bytecode");

    vm_task_destroy(task);
    vm_destroy(vm);
}

void test_vm_execute_control_flow(void) {
    Vm *vm = vm_create(4096);
    VmTask *task;
    VmInstruction *instructions;

    ASSERT_NOT_NULL(vm, "VM creation failed in control-flow test");
    vm->bytecode_size = 4 * sizeof(VmInstruction);
    vm->bytecode = vm_alloc(vm->bytecode_size);
    ASSERT_NOT_NULL(vm->bytecode, "Bytecode allocation failed");
    instructions = (VmInstruction *) vm->bytecode;
    instructions[0].opcode = VM_OP_JZ;
    instructions[0].src1 = 0;
    instructions[0].operand = 2;
    instructions[1].opcode = VM_OP_YIELD;
    instructions[2].opcode = VM_OP_JMP;
    instructions[2].operand = 3;
    instructions[3].opcode = VM_OP_HALT;

    task = vm_task_create(vm, 1024);
    ASSERT_NOT_NULL(task, "Task creation failed in control-flow test");
    ASSERT_EQ(vm_execute(task), 1, "Control-flow execution did not halt");
    ASSERT_EQ(task->pc, 4, "Control-flow execution ended at wrong PC");

    vm_task_destroy(task);
    vm_destroy(vm);
}

void test_vm_execute_shared_memory(void) {
    Vm *vm = vm_create(32);
    VmTask *task;
    VmInstruction *instructions;

    ASSERT_NOT_NULL(vm, "VM creation failed in shared memory test");
    vm->bytecode_size = 7 * sizeof(VmInstruction);
    vm->bytecode = vm_alloc(vm->bytecode_size);
    ASSERT_NOT_NULL(vm->bytecode, "Bytecode allocation failed");
    instructions = (VmInstruction *) vm->bytecode;
    instructions[0] = (VmInstruction) {VM_OP_MOVI, 0, 0, 0, 0};
    instructions[1] = (VmInstruction) {VM_OP_MOVI, 1, 0, 0, 7};
    instructions[2] = (VmInstruction) {VM_OP_STORE, 0, 0, 1, 0};
    instructions[3] = (VmInstruction) {VM_OP_LOAD, 2, 0, 0, 0};
    instructions[4] = (VmInstruction) {VM_OP_ATOMIC_ADD, 0, 0, 1, 0};
    instructions[5] = (VmInstruction) {VM_OP_CAS, 3, 0, 2, 11};
    instructions[6] = (VmInstruction) {VM_OP_HALT, 0, 0, 0, 0};

    task = vm_task_create(vm, 1024);
    ASSERT_NOT_NULL(task, "Task creation failed in shared memory test");
    ASSERT_EQ(vm_execute(task), 1, "Shared memory program did not halt");
    ASSERT_EQ(task->registers[2], 7, "LOAD returned incorrect value");
    ASSERT_EQ(task->registers[3], 7, "CAS returned incorrect old value");
    ASSERT_EQ(*(uint64_t *) vm->shared_mem, 11, "Atomic/CAS result is incorrect");

    vm_task_destroy(task);
    vm_destroy(vm);
}

void test_vm_rejects_invalid_shared_address(void) {
    Vm *vm = vm_create(8);
    VmTask *task;
    VmInstruction *instruction;

    ASSERT_NOT_NULL(vm, "VM creation failed in invalid address test");
    vm->bytecode_size = 2 * sizeof(VmInstruction);
    vm->bytecode = vm_alloc(vm->bytecode_size);
    instruction = (VmInstruction *) vm->bytecode;
    instruction[0] = (VmInstruction) {VM_OP_MOVI, 0, 0, 0, 1};
    instruction[1] = (VmInstruction) {VM_OP_LOAD, 1, 0, 0, 0};
    task = vm_task_create(vm, 1024);
    ASSERT_NOT_NULL(task, "Task creation failed in invalid address test");
    ASSERT_EQ(vm_execute(task), 0, "Invalid shared address was accepted");
    ASSERT_EQ(task->pc, 1, "PC did not stop at invalid memory access");
    vm_task_destroy(task);
    vm_destroy(vm);
}

void run_vm_tests(void) {
    RUN_TEST(test_vm_create_and_destroy);
    RUN_TEST(test_vm_task_create_and_destroy);
    RUN_TEST(test_vm_lifecycle);
    RUN_TEST(test_vm_execute_halt);
    RUN_TEST(test_vm_execute_rejects_invalid_bytecode);
    RUN_TEST(test_vm_execute_control_flow);
    RUN_TEST(test_vm_execute_shared_memory);
    RUN_TEST(test_vm_rejects_invalid_shared_address);
}
