#include "test_harness.h"

extern void run_allocator_tests(void);
extern void run_vm_tests(void);

void runtime_start(void *stack) {
    (void) stack;

    println("========================================\n");
    println("      HELL-64 PILTOVER TEST SUITE      \n");
    println("========================================\n");

    run_allocator_tests();
    run_vm_tests();

    println("----------------------------------------\n");
    if (g_tests_failed == 0) {
        println("ALL TESTS PASSED!\n");
        sys_exit(0);
    } else {
        println("SOME TESTS FAILED!\n");
        sys_exit(1);
    }
}
