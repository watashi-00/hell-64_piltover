//
// Created by watashi on 9/25/26.
//

#ifndef HELL_64_PILTOVER_ALLOCATOR_H
#define HELL_64_PILTOVER_ALLOCATOR_H
#include <stdint.h>

void *vm_alloc(uint64_t size);
void vm_free(void *ptr, uint64_t size);

#endif //HELL_64_PILTOVER_ALLOCATOR_H
