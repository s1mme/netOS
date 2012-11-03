;=======================================================  ;
;  gdt                                                    ;
;=======================================================  ; 
section .text
gdt_data_start: 
; every descriptor is 8 bytes !
; Null descriptor (Offset: 0x0)
	dd 0 				
	dd 0 
 
; Offset: 0x8 bytes)
	dw 0FFFFh 			
	dw 0 				
	db 0 				
	db 10011010b 			; access, ring 0
	db 11001111b 			
	db 0 				
 
; (Offset: 16 (0x10) bytes
	dw 0FFFFh 			 
	dw 0 				
	db 0 				
	db 10010010b 			
	db 11001111b 			; granularity
	db 0				
gdt_data_end:
gdt_pointer:
	dw gdt_data_end - gdt_data_start - 1
	dd gdt_data_start
	


gdt_flush:

	lgdt [gdt_pointer] ;reserve space for our gdt, but we also need to fill it with info

		
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush
flush:
	ret ; returns back to what?!
