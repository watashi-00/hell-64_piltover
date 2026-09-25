//
// Created by watashi on 9/25/26.
//

#ifndef HELL_64_PILTOVER_NATIVE_FUNCTIONS_H
#define HELL_64_PILTOVER_NATIVE_FUNCTIONS_H

long sys_write(int fd, const void *buf, long count);

long sys_read(int fd, void *buf, long count);

long sys_mmap(void *addr, long length, long prot,
              long flags, long fd, long offset);

long println_n(const char *str, long len);

#define println(str) println_n(str, sizeof(str))

long scan_n(void *buf, long count);

#define scan(buf) scan_n(buf, sizeof(buf))

#define bool _Bool
#define true 1
#define false 0

bool string_equals(const char *a, const char *b);

#endif //HELL_64_PILTOVER_NATIVE_FUNCTIONS_H
