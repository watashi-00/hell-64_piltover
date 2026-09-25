#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include "native_functions.h"

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            println("  [FAIL] Assertion failed: " msg "\n"); \
            g_tests_failed++; \
            return; \
        } \
    } while (0)

#define ASSERT_FALSE(cond, msg) \
    ASSERT_TRUE(!(cond), msg)

#define ASSERT_EQ(a, b, msg) \
    ASSERT_TRUE((a) == (b), msg)

#define ASSERT_NOT_NULL(ptr, msg) \
    ASSERT_TRUE((ptr) != 0, msg)

#define ASSERT_NULL(ptr, msg) \
    ASSERT_TRUE((ptr) == 0, msg)

#define RUN_TEST(test_func) \
    do { \
        println("[RUN] " #test_func "\n"); \
        int failed_before = g_tests_failed; \
        test_func(); \
        g_tests_run++; \
        if (g_tests_failed == failed_before) { \
            println("[PASS] " #test_func "\n"); \
        } \
    } while (0)

#endif // TEST_HARNESS_H
