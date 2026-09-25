.intel_syntax noprefix

.global sys_write
.global sys_read
.global sys_mmap
.global sys_munmap
.global sys_mprotect
.global sys_futex
.global sys_exit

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

sys_futex:
    mov rax, 202
    syscall
    ret

sys_exit:
    mov rax, 60
    syscall
    hlt