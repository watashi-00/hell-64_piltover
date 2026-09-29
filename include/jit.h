//
// Created by watashi on 9/28/26.
//

#ifndef HELL_64_PILTOVER_JIT_H
#define HELL_64_PILTOVER_JIT_H

#include <stdint.h>
#include "bytecode.h"
#include "vm.h"

typedef struct {
    uint8_t *data;
    uint64_t capacity;
    uint64_t size;
    int executable;
} JitCodeBuffer;

int jit_code_buffer_init(JitCodeBuffer *buffer, uint64_t capacity);
int jit_code_buffer_emit(JitCodeBuffer *buffer, uint8_t byte);
int jit_code_buffer_finalize(JitCodeBuffer *buffer);
void jit_code_buffer_destroy(JitCodeBuffer *buffer);

typedef uint64_t (*JitFunction)(void);
typedef void (*JitTaskFunction)(VmTask *task);

JitFunction jit_code_buffer_function(const JitCodeBuffer *buffer);
int jit_emit_return_42(JitCodeBuffer *buffer);
int jit_compile_block(JitCodeBuffer *buffer,
                      const VmInstruction *instructions,
                      uint64_t instruction_count);

#endif // HELL_64_PILTOVER_JIT_H