//
// Created by watashi on 9/28/26.
//

#include "jit.h"
#include "native_functions.h"
#include "bytecode.h"

#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

#define MAP_PRIVATE  0x02
#define MAP_ANONYMOUS 0x20

int jit_code_buffer_init(JitCodeBuffer *buffer, uint64_t capacity) {
    long result;

    if (buffer == 0 || capacity == 0)
        return 0;

    result = sys_mmap(0, (long) capacity, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (result < 0)
        return 0;

    buffer->data = (uint8_t *) result;
    buffer->capacity = capacity;
    buffer->size = 0;
    buffer->executable = 0;
    return 1;
}

int jit_code_buffer_emit(JitCodeBuffer *buffer, uint8_t byte) {
    if (buffer == 0 || buffer->data == 0 || buffer->executable ||
        buffer->size >= buffer->capacity)
        return 0;

    buffer->data[buffer->size++] = byte;
    return 1;
}

int jit_code_buffer_finalize(JitCodeBuffer *buffer) {
    if (buffer == 0 || buffer->data == 0 || buffer->size == 0 ||
        buffer->executable)
        return 0;

    if (sys_mprotect(buffer->data, (long) buffer->capacity,
                     PROT_READ | PROT_EXEC) < 0)
        return 0;

    buffer->executable = 1;
    return 1;
}

void jit_code_buffer_destroy(JitCodeBuffer *buffer) {
    if (buffer == 0 || buffer->data == 0)
        return;

    sys_munmap(buffer->data, (long) buffer->capacity);
    buffer->data = 0;
    buffer->capacity = 0;
    buffer->size = 0;
    buffer->executable = 0;
}

JitFunction jit_code_buffer_function(const JitCodeBuffer *buffer) {
    if (buffer == 0 || buffer->data == 0 || !buffer->executable)
        return 0;

    return (JitFunction) buffer->data;
}

static int emit_u64(JitCodeBuffer *buffer, uint64_t value) {
    for (int i = 0; i < 8; i++) {
        if (!jit_code_buffer_emit(buffer, (uint8_t) (value >> (i * 8))))
            return 0;
    }
    return 1;
}

static uint8_t register_offset(uint8_t reg) {
    return (uint8_t) (sizeof(Vm *) + reg * sizeof(uint64_t));
}

static int emit_load(JitCodeBuffer *buffer, uint8_t physical, uint8_t vm_reg) {
    return jit_code_buffer_emit(buffer, 0x48) &&
           jit_code_buffer_emit(buffer, 0x8b) &&
           jit_code_buffer_emit(buffer, (uint8_t) (0x47 | (physical << 3))) &&
           jit_code_buffer_emit(buffer, register_offset(vm_reg));
}

static int emit_store(JitCodeBuffer *buffer, uint8_t vm_reg, uint8_t physical) {
    return jit_code_buffer_emit(buffer, 0x48) &&
           jit_code_buffer_emit(buffer, 0x89) &&
           jit_code_buffer_emit(buffer, (uint8_t) (0x47 | (physical << 3))) &&
           jit_code_buffer_emit(buffer, register_offset(vm_reg));
}

int jit_compile_block(JitCodeBuffer *buffer,
                      const VmInstruction *instructions,
                      uint64_t instruction_count) {
    if (buffer == 0 || instructions == 0 || instruction_count == 0)
        return 0;

    for (uint64_t i = 0; i < instruction_count; i++) {
        const VmInstruction *instruction = &instructions[i];

        if (instruction->opcode == VM_OP_HALT)
            break;

        if (instruction->opcode == VM_OP_MOVI) {
            if (!jit_code_buffer_emit(buffer, 0x48) ||
                !jit_code_buffer_emit(buffer, 0xb8) ||
                !emit_u64(buffer, instruction->operand) ||
                !emit_store(buffer, instruction->dst, 0))
                return 0;
        } else {
            if (!emit_load(buffer, 0, instruction->src1))
                return 0;
            if (instruction->opcode == VM_OP_MOV) {
                if (!emit_store(buffer, instruction->dst, 0))
                    return 0;
            } else {
                if (!emit_load(buffer, 1, instruction->src2))
                    return 0;
                if (instruction->opcode == VM_OP_ADD) {
                    if (!jit_code_buffer_emit(buffer, 0x48) ||
                        !jit_code_buffer_emit(buffer, 0x01) ||
                        !jit_code_buffer_emit(buffer, 0xc8))
                        return 0;
                } else if (instruction->opcode == VM_OP_SUB) {
                    if (!jit_code_buffer_emit(buffer, 0x48) ||
                        !jit_code_buffer_emit(buffer, 0x29) ||
                        !jit_code_buffer_emit(buffer, 0xc8))
                        return 0;
                } else if (instruction->opcode == VM_OP_XOR) {
                    if (!jit_code_buffer_emit(buffer, 0x48) ||
                        !jit_code_buffer_emit(buffer, 0x31) ||
                        !jit_code_buffer_emit(buffer, 0xc8))
                        return 0;
                } else if (instruction->opcode == VM_OP_MUL) {
                    if (!jit_code_buffer_emit(buffer, 0x48) ||
                        !jit_code_buffer_emit(buffer, 0x0f) ||
                        !jit_code_buffer_emit(buffer, 0xaf) ||
                        !jit_code_buffer_emit(buffer, 0xc1))
                        return 0;
                } else {
                    return 0;
                }
                if (!emit_store(buffer, instruction->dst, 0))
                    return 0;
            }
        }
    }

    return jit_code_buffer_emit(buffer, 0xc3);
}

int jit_emit_return_42(JitCodeBuffer *buffer) {
    return jit_code_buffer_emit(buffer, 0x48) &&
           jit_code_buffer_emit(buffer, 0xb8) &&
           jit_code_buffer_emit(buffer, 42) &&
           jit_code_buffer_emit(buffer, 0) &&
           jit_code_buffer_emit(buffer, 0) &&
           jit_code_buffer_emit(buffer, 0) &&
           jit_code_buffer_emit(buffer, 0) &&
           jit_code_buffer_emit(buffer, 0) &&
           jit_code_buffer_emit(buffer, 0) &&
           jit_code_buffer_emit(buffer, 0) &&
           jit_code_buffer_emit(buffer, 0xc3);
}