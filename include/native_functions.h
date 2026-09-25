//
// Created by watashi on 9/25/26.
//

#ifndef HELL_64_PILTOVER_NATIVE_FUNCTIONS_H
#define HELL_64_PILTOVER_NATIVE_FUNCTIONS_H
#include <stdio.h>

ssize_t sys_write(int fd, const void *buf, size_t count);
ssize_t println_n(const char *str, size_t len);

#define println(str) println_n(str, sizeof(str))

#endif //HELL_64_PILTOVER_NATIVE_FUNCTIONS_H
