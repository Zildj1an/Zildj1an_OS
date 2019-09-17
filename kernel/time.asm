;############################################
; Timer
; Author Carlos Bilbao(Zildj1an)
;############################################

[BITS 32]

global asm_timer
global quit

SECTION .data
msg        db      'From 1970 (in seconds): ', 0h

SECTION .text
global  _start

asm_timer:

    mov     edx, 24     ; num of bytes to write
    mov     ecx, msg    ; Our string into ECX
    mov     ebx, 1      ; write to stdout file
    mov     eax, 4      ; invoke SYS_WRITE (kernel opcode 4)
    int     80h

    mov     eax, 13     ; invoke SYS_TIME (kernel opcode 13)
    int     80h         ; call the kernel
    call    printInt    ; call our integer printing function with linefeed
    call    quit        ; call our quit function

printInt:
    ret		        ; TODO

;
; void exit()
; Exit program and restore resources

quit:
    mov     ebx, 0
    mov     eax, 1
    int     80h
    ret
