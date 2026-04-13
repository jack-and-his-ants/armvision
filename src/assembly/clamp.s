.global _clampASM

.text
; int32_t clampASM(int32_t v, int32_t min, int32_t max);

_clampASM:
    cmp x0,x1
    blt lower
    cmp x0,x2
    bgt higher
    ret
    lower:
    mov x0, x1
    ret
    higher:
    mov x0, x2
    ret


