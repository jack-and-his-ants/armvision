.global _negative_neon

.text

;   typedef struct {
;       unsigned char *data;
;       int width;
;       int height;
;       int row_size;
;   } Image;

; void negative(Image * bmp);

_negative_neon:
    ldr x1, [x0]        ; char * data
    ldr w2, [x0, #8]    ; int width
    ldr w3, [x0, #12]   ; int height
    ldr w4, [x0, #16]   ; int row_size

    mov w12, w4
    add w13, w2, w2, lsl #1
    sub w12,w12,w13     ; padding
    mov w13, w2
    lsr w13,w13,#4      ; num neon iterations
    sub w2,w2,w13, lsl #4

    mov w5, wzr ; int y = 0
    mov x7, x1  ;  current address
    y_ptl:
        cmp w5, w3
        b.ge end_y_loop             ;  if y >= height: end y loop

        mov w6,wzr                  ; int x = 0
        x_ptl_neon:
            cmp w6,w13
            b.ge end_x_neon_loop

            ld3 {v0.16b,v1.16b,v2.16b},[x7]

            mvn v0.16b, v0.16b
            mvn v1.16b, v1.16b
            mvn v2.16b, v2.16b

            st3 {v0.16b,v1.16b,v2.16b},[x7]


            add x7, x7, #48         ; increase current address by 48
            add w6, w6, #1
            b x_ptl_neon
        end_x_neon_loop:

        mov w6,wzr
        x_ptl_norm:
            cmp w6, w2
            b.ge end_x_norm_loop         ; if x>= width: end x loop


            ldrb w8, [x7]
            ldrb w9, [x7, #1]
            ldrb w10, [x7, #2]

            mov w11, #255

            sub w8,w11,w8
            sub w9,w11,w9
            sub w10,w11,w10

            strb w8, [x7]
            strb w9, [x7, #1]
            strb w10, [x7, #2]

            add x7, x7, #3         ; increase current address by 3
            add w6, w6, #1
            b x_ptl_norm                ; branch to x ptl
        end_x_norm_loop:




        add x7, x7, w12, uxtw




        add w5,w5, #1                  ; increment y



        b y_ptl                     ; branch to y ptl
    end_y_loop:
    ret