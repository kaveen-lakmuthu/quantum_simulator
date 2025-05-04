section .text
global normalize_vector_avx

; void normalize_vector_avx(double* state, int size)
normalize_vector_avx:
    push rbp
    mov rbp, rsp
    mov rdi, rdi  ; State vector
    mov rsi, rsi  ; Size (n)

    vxorpd ymm1, ymm1, ymm1  ; Zero out ymm1 (sum register)

.loop:
    cmp rsi, 0
    je .compute_sqrt

    vmovupd ymm0, [rdi]  ; Load state vector elements
    vmulpd ymm2, ymm0, ymm0  ; Square elements
    vaddpd ymm1, ymm1, ymm2  ; Sum squares

    add rdi, 32
    sub rsi, 4
    jmp .loop

.compute_sqrt:
    vsqrtpd ymm1, ymm1  ; Compute sqrt(sum)
    vdivpd ymm0, ymm0, ymm1  ; Normalize each element
    vmovupd [rdi], ymm0  ; Store normalized vector

    pop rbp
    ret
