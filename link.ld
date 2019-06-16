/* 
	The linker makes sure assembly code and C code 
	are put together in a single binary file, with this script.
*/

OUTPUT_FORMAT("elf32-i386")
ENTRY(start_k)
phys = 0x00100000; SECTIONS {

   .text phys : AT(phys) {
	code = .;
        *(.text)
        *(.rodata*)
        . = ALIGN(4096 * 4);
   }
   .data : AT(phys + (data - code))
   {
        data = .;
        *(.data)
        . = ALIGN(4096);
   }
   .bss : AT(phys + (bss - code))
   {
        bss = .;
        *(.bss)
        . = ALIGN(4096);
   }
   end = .;
}
