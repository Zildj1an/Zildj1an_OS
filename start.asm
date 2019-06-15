; ############################################################
; Initial entry file.
; This file initializes the kernel, calls main function in C
; Author Carlos Bilbao (Zildj1an) 2019
; ############################################################

SECTION .text
[BITS 32]
[GLOBAL start]
[EXTERN main]

EXTERN code, bss, end

start:
	; start C code
	mov esp, stack
	call main
	; we shouldn't return
	sti
.idle:
	hlt
	jmp .idle


SECTION .data

; will use it

SECTION .bss

; other stacks
stack_start:
	resd 1024

stack: