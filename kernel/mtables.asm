; #########################################
; Load GDT and IDT
; Author: Pablo Villalobos (pablo-vs)
; #########################################

[BITS 32]

global load_gdt
global reload_segments
global load_idt

section .data	; We will hardcode the GDT in .data and then just load it

gdt_desc:		; GDT Descriptor: size and address of the gdt
	db 0	
	dw 0

idt_desc:
	db 0
	dw 0

section .text
	
load_gdt:			; Note that interrupts were already disabled in start.asm
	mov eax, [esp + 4]	; Set address (first arg)
   	mov [gdt_desc + 2], eax
   	mov ax, [esp + 8]	; Set size (second arg)
   	mov [gdt_desc], ax
   	lgdt [gdt_desc]
	ret

reload_segments:		; Set segment registers to our new GDT selectors
	jmp 0x08:reload_CS	; Set CS to code segment selector
reload_CS:
	mov ax, 0x10		; Set everyting else to data segment selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret


load_idt:
	mov eax, [esp + 4]	; Set address (first arg)
   	mov [idt_desc + 2], eax
   	mov ax, [esp + 8]	; Set size (second arg)
   	mov [idt_desc], ax
   	lidt [idt_desc]
	sti			; We can now enable interruptions
	ret
