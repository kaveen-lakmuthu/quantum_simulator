section .text
global parallel_apply_gate

; void parallel_apply_gate(double* real, double* imag, int qubit, int size)
parallel_apply_gate:
    push rbp
    mov rbp, rsp

    mov rdi, rdi   ; real part of state vector
    mov rsi, rsi   ; imaginary part of state vector
    mov rdx, rdx   ; target qubit
    mov rcx, rcx   ; size of the state vector

.loop:
    cmp rcx, 0
    je .done

    vmovupd ymm0, [rdi]  ; Load real part of state vector
    vmovupd ymm1, [rsi]  ; Load imaginary part of state vector

    vmulpd ymm0, ymm0, ymm1  ; Apply parallel gate operation

    vmovupd [rdi], ymm0  ; Store updated real values
    vmovupd [rsi], ymm1  ; Store updated imaginary values

    add rdi, 32
    add rsi, 32
    sub rcx, 4
    jmp .loop

.done:
    pop rbp
    ret
