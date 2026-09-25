//
// Created by watashi on 9/25/26.
//

#include <stdint.h>

#define PROT_READ   0x1
#define PROT_WRITE  0x2

#define MAP_PRIVATE    0x02
#define MAP_ANONYMOUS   0x20

void *vm_alloc(uint64_t size) {

    long result = sys_mmap(
        0,
        (long)size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
        );

}

