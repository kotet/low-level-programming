section .text

string_length:
    xor rax, rax
    .loop:
    cmp byte [rdi+rax], 0
    je .loop_end
    inc rax
    jmp .loop
    .loop_end:
    ret

print_string:
    push rdi
    call string_length
    pop rsi
    mov rdx, rax
    mov rax, 1
    mov rdi, 1
    syscall
    ret

print_char:
    ; rdi, rsi, rdx
    push rdi
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    mov rsi, rsp
    syscall
    inc rsp
    xor rax, rax
    ret

print_newline:
    xor rax, rax
    ret


print_uint:
    mov rax, rdi
    mov rdi, rsp
    dec rdi
    push 0 ; 8バイトの領域を0埋め
    sub rsp, 16
    mov r8, 10

    .loop:
    xor rdx, rdx
    idiv r8
    or rdx, 0x30
    dec rdi
    mov byte [rdi], dl
    test rax, rax
    jnz .loop

    call print_string

    add rsp, 24
    ret


print_int:
    xor rax, rax
    ret

string_equals:
    xor rax, rax
    ret


read_char:
    xor rax, rax
    ret 

read_word:
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    ret 


string_copy:
    push rdi
    push rsi
    push rdx
    call string_length
    pop rdx
    pop rsi
    pop rdi
    cmp rax, rdx
    jae .error ; rax >= rdx

    xor rcx, rcx
    push rbx
    .loop:
    cmp rcx, rdx
    ja .end ; rcx > rdx
    mov bl, byte [rdi+rcx]
    mov byte [rsi+rcx], bl
    inc rcx
    jmp .loop

    .end:
    pop rbx
    mov rax, rsi
    ret
    .error:
    xor rax, rax
    ret