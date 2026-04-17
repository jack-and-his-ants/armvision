.global _grayscale_neon

.text

;   typedef struct {
;       unsigned char *data;
;       int width;
;       int height;
;       int row_size;
;   } Image;

; void grayscale_neon(Image * bmp);

_grayscale_neon:
    ldr x1, [x0]        ; char * data
    ldr w2, [x0, #8]    ; int width
    ldr w3, [x0, #12]   ; int height
    ldr w4, [x0, #16]   ; int row_size

    mov w12, w4
    add w13, w2, w2, lsl #1
    sub w12,w12,w13     ; padding
    mov w13, w2
    lsr w13,w13,#4      ; num neon iterations
    sub w2,w2,w13, lsl #4   ; number of

    mov w5, wzr ; int y = 0
    mov x7, x1  ;  current address


    ; LOADING COLOR MULTIPLIERS
    ; BLUE
    mov w11, #29
    dup v17.8h, w11
    ; GREEN
    mov w11, #150
    dup v18.8h, w11
    ; RED
    mov w11, #77
    dup v19.8h, w11

    y_ptl:
        cmp w5, w3
        b.ge end_y_loop             ;  if y >= height: end y loop

        mov w6,wzr                  ; int x = 0
        x_ptl_neon:
            cmp w6,w13
            b.ge end_x_neon_loop

            ld3 {v0.16b,v1.16b,v2.16b},[x7] ; load color values to registers (16 pixels)

            ; dividing colors into two registers (8 bit -> 16 bit)
            ; BLUE
            uxtl v3.8h, v0.8b
            uxtl2 v4.8h, v0.16b
            ; GREEN
            uxtl v5.8h, v1.8b
            uxtl2 v6.8h, v1.16b
            ; RED
            uxtl v7.8h, v2.8b
            uxtl2 v16.8h, v2.16b

            ; Multiplying and adding colors: result = B * 29 + G * 150 + R * 77
            ; for each part of vector register and saving the result in v3 and v4
            mul v3.8h, v3.8h, v17.8h
            mul v4.8h, v4.8h, v17.8h


            mla v3.8h, v18.8h, v5.8h
            mla v4.8h, v18.8h, v6.8h


            mla v3.8h, v19.8h, v7.8h
            mla v4.8h, v19.8h, v16.8h

            ; Shift right result = result >> 8 ( weighted mean )
            ushr v3.8h, v3.8h, #8
            ushr v4.8h, v4.8h, #8
            ; move result back to one register and copy it to two other ones
            xtn  v0.8b, v3.8h
            xtn2 v0.16b, v4.8h

            mov v1.16b, v0.16b
            mov v2.16b, v0.16b

            ; Store the result

            st3 {v0.16b,v1.16b,v2.16b}, [x7]


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

            mov w11, #29
            mul w8,w8, w11             ; B

            mov w11, #150
            mul w9,w9, w11             ; G

            mov w11, #77
            mul w10,w10, w11            ; R

            add w8,w8, w9
            add w8,w8, w10             ; sum

            lsr w8, w8, #8

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
    ret