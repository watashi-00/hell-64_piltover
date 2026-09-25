.intel_syntax noprefix

.global sys_write
.global sys_read
.type sys_write, @function
.type sys_read, @function

sys_write:
    mov rax, 1
    syscall
    ret

sys_read:
    mov rax, 0
    syscall
    ret