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
    mov rdi, rsp
    call print_string
    pop rdi
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
    test rdi, rdi
    jns print_uint
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    neg rdi
    jmp print_uint
    

string_equals:
    xor rax, rax
    ret


read_char:
    dec rsp ; バッファを確保
    xor rax, rax ; 0 (read)
    xor rdi, rdi ; 0 (stdin)
    mov rsi, rsp
    mov rdx, 1 ; 1 byte
    syscall
    test rax, rax
    jz .end
    mov al, byte [rsp]
    .end:
    inc rsp
    ret 

read_word: ; (char* ptr, int len)
    xor rdx, rdx

    .skip_spaces:
    push rdi
    push rsi
    push rdx
    call read_char
    pop rdx
    pop rsi
    pop rdi
    cmp al, 0x20
    je .skip_spaces
    cmp al, 0x09
    je .skip_spaces
    cmp al, 0x0A
    je .skip_spaces
    cmp al, 0x0D
    je .skip_spaces
    test al, al
    jz .success

    .read_word:
    mov byte [rdi + rdx], al
    inc rdx
    push rdi
    push rsi
    push rdx
    call read_char
    pop rdx
    pop rsi
    pop rdi
    cmp al, 0x20
    je .success
    cmp al, 0x09
    je .success
    cmp al, 0x0A
    je .success
    cmp al, 0x0D
    je .success
    test al, al
    jz .success
    cmp rdx, rsi
    je .error
    jmp .read_word

    .success:
    mov byte [rdi + rdx], 0
    mov rax, rdi
    ret

    .error:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    mov r8, 10
    xor rax, rax
    xor rcx, rcx
    .loop:
    movzx r9, byte [rdi + rcx]
    cmp r9b, '0'
    jb .end
    cmp r9b, '9'
    ja .end
    xor rdx, rdx
    mul r8
    sub r9, '0'
    add rax, r9
    inc rcx
    jmp .loop
    .end:
    mov rdx, rcx
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    movzx r8, byte[rdi]
    cmp r8, '-'
    je .neg
    jmp parse_uint
    .neg:
    inc rdi
    call parse_uint
    inc rdx
    neg rax
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