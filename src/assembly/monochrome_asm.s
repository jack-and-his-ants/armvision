.global _monochrome_asm

.text


;   typedef struct {
;       unsigned char *data;
;       int width;
;       int height;
;       int row_size;
;   } Image;

; void monochrome_asm(Image * bmp,char channel);

_monochrome_asm:
    cmp w1,'b'
    beq monochrome_b
    cmp w1,'g'
    beq monochrome_g
    cmp w1,'r'
    beq monochrome_r

    mov x0,#1
    end_subroutine:
    ret

monochrome_b:
    start_b:
    ldr x7, [x0]        ; char * data
    ldr w2, [x0, #8]    ; int width
    ldr w3, [x0, #12]   ; int height
    ldr w4, [x0, #16]   ; int row_size

    mov w5, wzr ; int y = 0
    y_ptl_b:
        cmp w5, w3
        b.ge end_y_loop_b             ;  if y >= height: end y loop

        mov w6,wzr                  ; int x = 0

        x_ptl_b:
            cmp w6, w2
            b.ge end_x_loop_b         ; if x>= width: end x loop

            mov w9, #0
            mov w10, #0         ; erasing red and green colors

            strb w9, [x7, #1]
            strb w10, [x7, #2]

            add w6,w6, #1              ; increment x
            add x7,x7, #3              ; increase current address by 3
            b x_ptl_b                 ; branch to x ptl
        end_x_loop_b:

        add w5,w5, #1                  ; increment y
        b y_ptl_b                     ; branch to y ptl
    end_y_loop_b:

    mov x0, #0
    end_subroutine_b:
    ret


monochrome_g:
    start_g:
    ldr x7, [x0]        ; char * data
    ldr w2, [x0, #8]    ; int width
    ldr w3, [x0, #12]   ; int height
    ldr w4, [x0, #16]   ; int row_size

    mov w5, wzr ; int y = 0
    y_ptl_g:
        cmp w5, w3
        b.ge end_y_loop_g            ;  if y >= height: end y loop

        mov w6,wzr                  ; int x = 0

        x_ptl_g:
            cmp w6, w2
            b.ge end_x_loop_g         ; if x>= width: end x loop

            mov w8, #0
            mov w10, #0         ; erasing red and green colors


            strb w8, [x7]
            strb w10, [x7, #2]

            add w6,w6, #1              ; increment x
            add x7,x7, #3              ; increase current address by 3
            b x_ptl_g                 ; branch to x ptl
        end_x_loop_g:

        add w5,w5, #1                  ; increment y
        b y_ptl_g                     ; branch to y ptl
    end_y_loop_g:

    mov x0, #0
    end_subroutine_g:
    ret

monochrome_r:
    start_r:
    ldr x7, [x0]        ; char * data
    ldr w2, [x0, #8]    ; int width
    ldr w3, [x0, #12]   ; int height
    ldr w4, [x0, #16]   ; int row_size

    mov w5, wzr ; int y = 0
    y_ptl_r:
        cmp w5, w3
        b.ge end_y_loop_r            ;  if y >= height: end y loop

        mov w6,wzr                  ; int x = 0

        x_ptl_r:
            cmp w6, w2
            b.ge end_x_loop_r         ; if x>= width: end x loop


            mov w8, #0
            mov w9, #0         ; erasing blue and green colors


            strb w8, [x7]
            strb w9, [x7, #1]

            add w6,w6, #1              ; increment x
            add x7,x7, #3              ; increase current address by 3
            b x_ptl_r                 ; branch to x ptl
        end_x_loop_r:

        add w5,w5, #1                  ; increment y
        b y_ptl_r                     ; branch to y ptl
    end_y_loop_r:

    mov x0, #0
    end_subroutine_r:
    ret