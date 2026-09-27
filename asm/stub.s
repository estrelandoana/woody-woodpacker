BITS 64

global _start

_start:
    push rax
    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9
    push r10
    push r11
    mov rax, 1                  ; syscall 1 (write)
    mov rdi, 1                  ; fd 1 (stdout)
    lea rsi, [rel woody_msg]    ; str addr
    mov rdx, 14                 ; str size
    syscall
    lea r9, [rel _start]
    mov r8,     0x1111111111111111  ; ph xor key
    mov rcx,    0x2222222222222222  ; ph .text size
    mov rdi,    0x3333333333333333  ; ph .text v-addr
    add rdi, r9
    push rdi
    push rcx
    mov rsi, rdi
    add rsi, rcx
    and rdi, -4096
    sub rsi, rdi
    mov rax, 10
    mov rdx, 7
    syscall
    pop rcx
    pop rdi

decrypt_loop:
    cmp rcx, 8
    jl remainder_loop
    xor [rdi], r8
    add rdi, 8
    sub rcx, 8
    jmp decrypt_loop

remainder_loop:
    test rcx, rcx
    jz end_decrypt
    xor [rdi], r8b
    shr r8, 8
    inc rdi
    dec rcx
    jmp remainder_loop

end_decrypt:
    mov rax, 0x4444444444444444
    add rax, r9
    pop r11
    pop r10
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    xchg rax, [rsp]
    ret

woody_msg:
    db "....WOODY....", 10