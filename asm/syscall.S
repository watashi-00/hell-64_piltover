.intel_syntax noprefix

.global sys_write
.global sys_read

.macro function name
    .type \name, @function
\name:
.endm

sys_write:
    mov rax, 1
    syscall
    ret

sys_read:
    mov rax, 0
    syscall
    ret

sys_mmap:
    mov rax, 9
    syscall
    ret

sys_munmap:
    mov rax, 11
    syscall
    ret

sys_mprotect:
    mov rax, 10
    sycall
    ret