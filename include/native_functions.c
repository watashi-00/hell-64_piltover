//
// Created by watashi on 9/25/26.
//

#include "native_functions.h"

long println_n(const char *str, long len);

long scan_n(void *buf, long len);

bool string_equals(const char *str1, const char *str2);

long println_n(const char *str, long len) {
    for (long i = 0; i < len; i++) {
        if (str[i] == '\0')
            return sys_write(1, str, i);
        if (str[i] == '\n')
            return sys_write(1, str, i + 1);
    }
    return sys_write(1, str, len);
}

long scan_n(void *buf, long len) {
    long n = sys_read(0, buf, len - 1);

    if (n <= 0)
        return n;

    char *str = buf;

    for (long i = 0; i < n; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            return i;
        }
    }

    str[n] = '\0';
    return n;
}

bool string_equals(const char *str1, const char *str2) {
    long i = 0;

    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i])
            return false;
        i++;
    }

    return str1[i] == str2[i];
}
