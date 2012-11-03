;=======================================================  ;
;  PIC                                             ;
;=======================================================  ; 
align 4
section .text
remap_pic:	
	cli
	pushad                                            ;
	push    es                                        ;
	push    ds  
	
	
	
	mov al, 0x11
	out 0x20, al
	
	mov al, 0x11
	out 0xa0, al
	
	mov al, 0x20
	out 0x21, al

	mov al, 0x28
	out 0xa1, al

	mov al, 0x04
	out 0x21, al

	mov al, 0x02
	out 0xa1, al
	
	mov al, 0x01
	out 0x21, al

	mov al, 0x01
	out 0xa1, al
	
	mov al, 0x0
	out 0x21, al
	
	mov al, 0x0
	out 0xa1, al
	
	pop     ds                                        ;
	pop     es                                        ;
	popad 
	sti
ret
