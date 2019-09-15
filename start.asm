; ############################################################
; Initial entry file.
; This file initializes the kernel, calls main function in C
; Author Carlos Bilbao (Zildj1an) 2019
; ############################################################

[BITS 32]
[global start_k]
[extern main_k]

; GRUB Multiboot Header
align 4
dd		0x1BADB002
dd		0x00
dd		- (0x1BADB002+0x00)

;===============
; SECTION text
;===============

section .text

start_k:
	cli		; Clean interrupts
	mov esp,stack
	push ebx	; Pass pointer to Boot Information to main_k
	; start C code
	call main_k
	; we shouldn't return
	sti
.idle:
	hlt
	jmp .idle

;===============
; SECTION data
;===============

section .data

msg db '> Zildj1an OS (Author Carlos Bilbao)', 0

;===============
; SECTION bss
;===============

section .bss

; Stacks

stack_start:
	resd 1024

stack:
