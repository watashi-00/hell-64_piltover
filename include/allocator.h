//
// Created by watashi on 9/25/26.
//

#ifndef HELL_64_PILTOVER_ALLOCATOR_H
#define HELL_64_PILTOVER_ALLOCATOR_H
#include <stdint.h>

void *vm_alloc(uint64_t size);

long vm_free_n(void *ptr, uint64_t size);

#define vm_free(ptr, size) vm_free_n(ptr, size)

#endif //HELL_64_PILTOVER_ALLOCATOR_H
