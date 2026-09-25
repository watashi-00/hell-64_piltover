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

function sys_write
    mov rax, 1
    syscall
    ret

function sys_read
    mov rax, 0
    syscall
    ret

function sys_mmap
    mov rax, 9
    syscall
    ret

function sys_munmap
    mov rax, 11
    syscall
    ret

function sys_mprotect
    mov rax, 10
    syscall
    ret

function sys_futex
    mov rax, 202
    syscall
    ret

function sys_exit
    mov rax, 60
    syscall
    hlt