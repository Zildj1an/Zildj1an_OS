;############################################
; Timer
; Author Carlos Bilbao(Zildj1an)
;############################################

[BITS 32]

[global asm_timer]
[global quit]
[extern write_O]

SECTION .data
msg        db      'From 1970 (in seconds): ', 0h

SECTION .text
global  _start

asm_timer:

    push msg        	; msg
    push 24		; count
    push 1 		; color
    call    write_O

    mov     rax, 13     ; invoke SYS_TIME (kernel opcode 13)
    int     80h         ; call the kernel
    call    printInt    ; call our integer printing function with linefeed
    call    quit        ; call our quit function

printInt:
    ret		        ; TODO

;
; void exit()
; Exit program and restore resources

quit:
    mov     rbx, 0
    mov     rax, 1
    int     80h
    ret
