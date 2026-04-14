.global _grayscale_asm

.text


;   typedef struct {
;       unsigned char *data;
;       int width;
;       int height;
;       int row_size;
;   } Image;

; void grayscale(Image * bmp);

_grayscale_asm:
    ldr x1, [x0]        ; char * data
    ldr w2, [x0, #8]    ; int width
    ldr w3, [x0, #12]   ; int height
    ldr w4, [x0, #16]   ; int row_size

    mov w5, wzr ; int y = 0
    mov x7, x1  ;  current address
    y_ptl:
        cmp w5, w3
        b.ge end_y_loop             ;  if y >= height: end y loop

        mov w6,wzr                  ; int x = 0

        x_ptl:
            cmp w6, w2
            b.ge end_x_loop         ; if x>= width: end x loop


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

            add w6,w6, #1              ; increment x
            add x7,x7, #3              ; increase current address by 3
            b x_ptl                 ; branch to x ptl
        end_x_loop:

        add w5,w5, #1                  ; increment y
        b y_ptl                     ; branch to y ptl
    end_y_loop:
    ret