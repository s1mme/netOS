;=======================================================  ;
;  ISR's                                                
;=======================================================  ; 
section .text
align 4
div_error:
cli
	pushad                                            ;
	push    es                                        ;
	push    ds   
;mov dx, 0x64
;mov ax, 0xFE
;out dx, ax
mov esi, isrmsg
call putstring
mov eax, [esp - 8]
call puthex
pop     ds                                        ;
	pop     es                                        ;
	popad   
	sti
	hlt                                          ;
	iret                                              ;



;=======================================================  ;
;  General protection fault!                                                 ;
;=======================================================  ; 

gpf_error:
cli
	pushad                                            ;
	push    es                                        ;
	push    ds   
;mov dx, 0x64
;mov ax, 0xFE
;out dx, ax
mov esi, isrmsg2
call putstring
mov eax, [esp - 8]
call puthex
pop     ds                                        ;
	pop     es                                        ;
	popad   
	
	hlt  
	                                        ;
	iret                                              ;
	
align 4


;=======================================================  ;
;  Page fault!                                                ;
;=======================================================  ; 

page_fault:
cli
	pushad                                            ;
	push    es                                        ;
	push    ds   

	mov esi, isrmsg3
	call putstring
	
	
mov eax, [esp - 8]
call puthex
	pop     ds                                        ;
	pop     es                                        ;
	popad  
	
	hlt                                          ;
	iret                                              ;
	
align 4
isrmsg3 db "Page fault!", 0
isrmsg2 db "General Protection Fault!", 0	
isrmsg db "Divide Error Exception!", 0 
