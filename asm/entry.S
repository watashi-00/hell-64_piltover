.intel_syntax noprefix

.global _start

.type _start, @function

.section .text

_start:
    mov rdi, rsp
    call runtime_start

    mov rax, 60
    xor rdi, rdi
    syscall