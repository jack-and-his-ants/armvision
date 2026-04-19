.global _monochrome_neon

.text

;   typedef struct {
;       unsigned char *data;
;       int width;
;       int height;
;       int row_size;
;   } Image;

; void monochrome_neon(Image * bmp, char channel);

_monochrome_neon:
    ; x0 contains memory address of Image structure
    ; x1 contains channel
    ldr x7, [x0]        ; char * data and current address
    ldr w2, [x0, #8]    ; int width
    ldr w3, [x0, #12]   ; int height
    ldr w4, [x0, #16]   ; int row_size

    mov w12, w4
    add w13, w2, w2, lsl #1
    sub w12,w12,w13     ; padding
    mov w13, w2
    lsr w13,w13,#4      ; num neon iterations
    sub w2,w2,w13, lsl #4   ; number of regular iterations

    mov w5, wzr ; int y = 0

    ; free registers: x4,x14,x15

    ; LOADING COLOR MASKS
    ; BLUE
    cmp w1,'b'
    beq set_blue_mask
    cmp w1,'g'
    beq set_green_mask
    cmp w1,'r'
    beq set_red_mask

    mov x0,#1
    b end_monochrome

    set_blue_mask:
    ; v3 v4 v5
        mov w4, 0xff
        dup v3.16b, w4
        mov w14, wzr
        dup v4.16b, w14
        mov w15, wzr
        dup v5.16b, w15

    b end_set_mask
    set_green_mask:
        mov w4, wzr
        dup v3.16b, w4
        mov w14, 0xff
        dup v4.16b, w14
        mov w15, wzr
        dup v5.16b, w15

    b end_set_mask
    set_red_mask:
        mov w4, wzr
        dup v3.16b, w4
        mov w14, wzr
        dup v4.16b, w14
        mov w15, 0xff
        dup v5.16b, w15
    b end_set_mask
    end_set_mask:

    y_ptl:
        cmp w5, w3
        b.ge end_y_loop             ;  if y >= height: end y loop

        mov w6,wzr                  ; int x = 0
        x_ptl_neon:
            cmp w6,w13
            b.ge end_x_neon_loop

            ld3 {v0.16b,v1.16b,v2.16b},[x7] ; load color values to registers (16 pixels = 48 bytes)

            and v0.16b, v0.16b, v3.16b ; B
            and v1.16b, v1.16b, v4.16b ; G
            and v2.16b, v2.16b, v5.16b ; R

            st3 {v0.16b,v1.16b,v2.16b}, [x7] ; Store the result


            add x7, x7, #48         ; increase current address by 48
            add w6, w6, #1          ; increase x
            b x_ptl_neon
        end_x_neon_loop:

        mov w6,wzr
        x_ptl_norm:
            cmp w6, w2
            b.ge end_x_norm_loop         ; if x>= width: end x loop


            ldrb w8, [x7]
            ldrb w9, [x7, #1]
            ldrb w10, [x7, #2]

            and w8,w8,w4
            and w9,w9,w14
            and w10,w10,w15

            strb w8, [x7]
            strb w8, [x7, #1]
            strb w8, [x7, #2]

            add x7, x7, #3         ; increase current address by 3
            add w6, w6, #1
            b x_ptl_norm                ; branch to x ptl
        end_x_norm_loop:




        add x7, x7, w12, uxtw




        add w5,w5, #1                  ; increment y



        b y_ptl                     ; branch to y ptl
    end_y_loop:
    mov x0, xzr
    end_monochrome:
    ret