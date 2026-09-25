.intel_syntax noprefix

.global _start
.global sys_write

.type _start, @function
.type sys_write, @function

.section .text

_start:
    mov rdi, rsp
    call runtime_start

    mov rax, 60
    xor rdi, rdi
    syscall

sys_write:
    mov rax, 1
    syscall
    ret