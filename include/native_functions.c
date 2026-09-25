//
// Created by watashi on 9/25/26.
//

#include "native_functions.h"

long println_n(const char *str, size_t len);

long println_n(const char *str, size_t len) {

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\0')
            return sys_write(1, str, i);
        if (str[i] == '\n')
            return sys_write(1, str, i + 1);
    }
    return sys_write(1, str, len);
}