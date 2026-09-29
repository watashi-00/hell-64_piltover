#include "test_harness.h"
#include "jit.h"

void test_jit_return_42(void) {
    JitCodeBuffer buffer;
    JitFunction function;

    ASSERT_TRUE(jit_code_buffer_init(&buffer, 4096),
                "JIT code buffer initialization failed");
    ASSERT_TRUE(jit_emit_return_42(&buffer),
                "JIT minimal function emission failed");
    ASSERT_TRUE(jit_code_buffer_finalize(&buffer),
                "JIT code buffer finalization failed");

    function = jit_code_buffer_function(&buffer);
    ASSERT_NOT_NULL(function, "JIT function pointer is NULL");
    ASSERT_EQ(function(), 42, "JIT function returned the wrong value");

    jit_code_buffer_destroy(&buffer);
}

void test_jit_register_block(void) {
    JitCodeBuffer buffer;
    VmInstruction instructions[7] = {0};
    VmTask task = {0};
    JitTaskFunction function;

    instructions[0].opcode = VM_OP_MOVI;
    instructions[0].dst = 0;
    instructions[0].operand = 7;
    instructions[1].opcode = VM_OP_MOVI;
    instructions[1].dst = 1;
    instructions[1].operand = 5;
    instructions[2].opcode = VM_OP_ADD;
    instructions[2].dst = 0;
    instructions[2].src1 = 0;
    instructions[2].src2 = 1;
    instructions[3].opcode = VM_OP_SUB;
    instructions[3].dst = 0;
    instructions[3].src1 = 0;
    instructions[3].src2 = 1;
    instructions[4].opcode = VM_OP_MUL;
    instructions[4].dst = 0;
    instructions[4].src1 = 0;
    instructions[4].src2 = 1;
    instructions[5].opcode = VM_OP_XOR;
    instructions[5].dst = 0;
    instructions[5].src1 = 0;
    instructions[5].src2 = 1;
    instructions[6].opcode = VM_OP_HALT;

    ASSERT_TRUE(jit_code_buffer_init(&buffer, 4096),
                "JIT register block buffer initialization failed");
    ASSERT_TRUE(jit_compile_block(&buffer, instructions, 7),
                "JIT register block compilation failed");
    ASSERT_TRUE(jit_code_buffer_finalize(&buffer),
                "JIT register block finalization failed");
    function = (JitTaskFunction) jit_code_buffer_function(&buffer);
    ASSERT_NOT_NULL(function, "JIT task function pointer is NULL");
    function(&task);
    ASSERT_EQ(task.registers[0], 38, "JIT arithmetic result is incorrect");
    ASSERT_EQ(task.registers[1], 5, "JIT source register is incorrect");
    jit_code_buffer_destroy(&buffer);
}

void run_jit_tests(void) {
    RUN_TEST(test_jit_return_42);
    RUN_TEST(test_jit_register_block);
}