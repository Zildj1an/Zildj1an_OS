;=============
SECTION .text
;=============
[BITS 32]
[GLOBAL start_k]
[EXTERN main_k]
EXTERN code, bss, end
start_k:
mov esp, stack
call main_k
sti
.idle:
hlt
jmp .idle
;=============
SECTION .data
;=============
; set up things like IDT, GDT or ISR/IRQ definitions
;=============
SECTION .bss
;=============
; add other stacks like timerstack
stack_start:
resd 1024
stack:
