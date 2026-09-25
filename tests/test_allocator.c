#include "test_harness.h"
#include "allocator.h"

void test_allocator_basic_alloc(void) {
    void *ptr = vm_alloc(1024);
    ASSERT_NOT_NULL(ptr, "vm_alloc(1024) returned NULL");

    long free_res = vm_free_n(ptr, 1024);
    ASSERT_EQ(free_res, 0, "vm_free_n failed");
}

void test_allocator_multiple_allocs(void) {
    void *p1 = vm_alloc(512);
    void *p2 = vm_alloc(2048);

    ASSERT_NOT_NULL(p1, "vm_alloc p1 failed");
    ASSERT_NOT_NULL(p2, "vm_alloc p2 failed");

    ASSERT_EQ(vm_free_n(p1, 512), 0, "vm_free_n p1 failed");
    ASSERT_EQ(vm_free_n(p2, 2048), 0, "vm_free_n p2 failed");
}

void run_allocator_tests(void) {
    RUN_TEST(test_allocator_basic_alloc);
    RUN_TEST(test_allocator_multiple_allocs);
}
