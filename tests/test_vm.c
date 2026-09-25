#include "test_harness.h"
#include "vm.h"

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
    ASSERT_EQ(task->stack_size, 2048, "task stack_size mismatch");
    ASSERT_NOT_NULL(task->stack, "task stack is NULL");

    task->registers[0] = 42;
    ASSERT_EQ(task->registers[0], 42, "register value failed");

    vm_task_destroy(task);
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

void run_vm_tests(void) {
    RUN_TEST(test_vm_create_and_destroy);
    RUN_TEST(test_vm_task_create_and_destroy);
    RUN_TEST(test_vm_lifecycle);
}
