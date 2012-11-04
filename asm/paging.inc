;=======================================================  ;
;  Paging module! todo: make a bitmap page allocator?                                                    ;
;=======================================================  ; 
[bits 32]
section .text
init_paging:
cli
	pusha 
    push ds
    push es
    push fs
    push gs
   ;Create a basic page table
   mov   ecx, 1024		;how many pages to map to a frames in 1 table?, 1024 i maximum because 1024*1024*4 is 4194304 bytes!
   mov   edi, 0x9d000      ;address of the page table 4 kb right after pde
   mov   edx, 0b00000000000000000000000000000011             ;set attributes, 00000000 00000000 00 000000 00000111 <- user1/kernel0, read0/write1 and present1, 
			    									;we start with 20 bit frame adress: 00000000 00000000 0000 then moves with 1 page size / per loop!	
.loop1:
   mov   [edi], edx            ; for every loop we put edx as attribut
   add   edi, 4              ;go to next memory location, 4 bytes next because the descriptor is 32 bits
   add   edx, 4096 			;go to next frame adress!
   cmp ecx, 700				
   jz .mmio
   jmp .next
.mmio:
				  
	  mov edx,0xf0200000	;dword [pciio]	; pcimem, holds 0xf2001000, or 0b11110010000000000001000000000000
	  or edx, 1
.calc:	
 	;  MMIO adress @ 325*4096 - 0x145000
   mov   [edi], edx            ; attribut
   add   edi, 4  
   add edx, 4096
   
   push edx
   xor edx, edx
	mov eax, ecx
		mov ebx, 1024
	div ebx
	mov ecx, edx
   pop edx  
   
   dec ecx
   cmp ecx, 0
   jz .calc
.next:	
   
   
   dec   ecx
   jnz   .loop1        ;looping tills we reach 0x9e000

   ;Creating Page Directories!
   mov   ecx, 1024			
   mov   edi, 0x9C000      ;address of the page directory 4 kb before pte
   mov   edx, 0             
.loop2:
   mov   [edi], edx           ;for every loop we put edx as attribut
   add   edi, 4             ;go to next memory location
   dec   ecx
   jnz   .loop2 				;looping tills we reach 0x9d000, page table size + directory = 8kb
   
   mov ecx, 0
.loop3:
   mov ebx, 0x3

   mov dword [edx+ecx], 0x9d000
   add dword [edx+ecx], ebx
   add ebx, 0x4
   add ecx, 0x4
   cmp ecx, 500 ;megabyte counter, total 512 mb or mapping 536870912 bytes :D 
   jne .loop3
   
   mov   cr3, edx
   mov   eax, cr0				;enable paging! :D
   or    eax, 0x80000000
   mov   cr0, eax       
				
					  pop gs
    pop fs
    pop es
    pop ds

 popa 	
	sti
ret	

	

