section .text
global matrix_multiply_avx

; void matrix_multiply_avx(double* A, double* B, double* C, int size)
matrix_multiply_avx:
    ; Prologue
    push rbp
    mov rbp, rsp
    mov rdi, rdi  ; A (Matrix)
    mov rsi, rsi  ; B (Vector)
    mov rdx, rdx  ; C (Result Vector)
    mov rcx, rcx  ; Size (n)

.loop:
    cmp rcx, 0
    je .done

    vmovupd ymm0, [rsi]  ; Load B into AVX register
    vmovupd ymm1, [rdi]  ; Load row of A into AVX register
    vmulpd ymm2, ymm0, ymm1  ; Element-wise multiply
    vaddpd ymm3, ymm3, ymm2  ; Sum results

    vmovupd [rdx], ymm3  ; Store result in C

    add rdi, 32  ; Move to next row of A
    add rdx, 32  ; Move to next row of C
    sub rcx, 4  ; Process 4 elements at a time
    jmp .loop

.done:
    pop rbp
    ret
