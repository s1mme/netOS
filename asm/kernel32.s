;=======================================================  ;
;  kernel entry point!                                                    ;
;=======================================================  ; 
[bits 32]

GRUB_MAGIC equ	0x1BADB002 ;lets the bootloader find the header
GRUB_FLAGS equ	10b
GRUB_CHECK equ	-(GRUB_MAGIC + GRUB_FLAGS)

section .text

align 4
	dd	GRUB_MAGIC
	dd	GRUB_FLAGS
	dd	GRUB_CHECK
		
			
	mov esp, 0x190000         ; set up the stack
	;cli
	push ebx
	mov byte [attrib], 0
	call clear_screen
	mov  dword [tick_count], 0x0
	mov esi, gdtmsg
	call putstring_nonewline
	call gdt_flush
	mov byte [attrib], 0x09
	mov esi, okmsg
	call putstring_nonewline
	mov byte [attrib], 0
	add byte [ypos], 1
	mov byte [xpos], 0
	mov esi, pciscan
	call putstring
	call pci_check
	call putstring
	mov byte [attrib], 0x09
	mov esi, donemsg
	call putstring
	mov byte [attrib], 0
	mov esi, pagingmsg
	call putstring_nonewline
	call init_paging
	mov byte [attrib], 0x09
	mov esi, okmsg
	call putstring_nonewline
	mov byte [attrib], 0
	
	;call install_rtl8139
	
	add byte [ypos], 1
	mov byte [xpos], 0
	mov esi, handlemsg
	call putstring_nonewline
	call install_handles
	mov byte [attrib], 0x09
	mov esi, okmsg
	call putstring_nonewline
	mov byte [attrib], 0
	add byte [ypos], 1
	mov byte [xpos], 0
	call remap_pic
	mov esi, idtmsg
	call putstring_nonewline
		
	call idt_flush
	
	mov byte [attrib], 0x09
	mov esi, okmsg
	call putstring_nonewline
	mov byte [attrib], 0
	;mov eax, 1440			;720 / 180 = total seconds
	;call delay
	mov dword [buffercounter], 0x0
	

	add byte [ypos], 1
	mov byte [xpos],0 

	mov esi, timermess
	call putstring_nonewline
	
	
	call timer_install
	mov byte [attrib], 0x09
	mov esi, okmsg
	call putstring_nonewline
	mov byte [attrib], 0
	add byte [ypos], 1
	mov byte [xpos],0 
	mov esi, computerinfo
	call putstring
	pop ebx	;multiboot structure!
	mov eax, dword [ebx+4]
	add eax, dword [ebx+8]
	mov ebx, 1024
	xor edx, edx
	div ebx
	push eax
	mov esi, mem
	call putstring
	pop eax
	call putint
	mov esi, cputype
	call putstring
	call cpu1
	
	
	
	mov esi, shelluser
	call putstring_nonewline
	
				 
	;mov eax, [0x145000] ;page fault test #2, we have mapped 1024*1024*4*(512/4) = 536870912 bytes :D that is 0x20000000 <- this will give page fault!

	;mov ebx, 0 ;test exception #0
	;div ebx
	
jmp $
%include "asm/screen.inc"
%include "asm/misc.inc"
%include "asm/gdt.inc"
%include "asm/idt.inc"
%include "asm/isr.inc"
%include "asm/8259.inc"
%include "asm/handles.inc"
%include "asm/8253timer.inc"
%include "asm/kb.inc"
%include "asm/paging.inc"
%include "asm/pci.inc"
%include "asm/rtl8139.inc"
;%include "asm/multitasking.inc"
%include "asm/arp.inc"
%include "asm/icmp.inc"
%include "asm/algoritm.inc"
%include "asm/udp.inc"
%include "asm/tcp.inc"
%include "asm/shell.inc"
mem: db "Available memory:", 0
okmsg: db "[ok]", 0
pciscan: db "Scanning pci bus...", 0
gdtmsg: db "Initializing GDTs... ", 0
msg: db "Hello! this is a test string!", 0
msg2: db "CPU type :", 0 
donemsg: db "[Done]", 0
idtmsg: db "Flushing IDTs...", 0 
errorprintk: db "error wrong type", 0
pagingmsg: db "Initializing paging...", 0
handlemsg: db "Setting handles up...", 0
timermess: db "Installing timer...", 0
computerinfo: db "Computer info:", 0
cputype: db "CPU type:", 0
