.intel_syntax noprefix

.global sys_write
.type sys_write, @function

sys_write:
    mov rax, 1
    syscall
    ret