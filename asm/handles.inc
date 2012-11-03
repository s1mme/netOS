;=======================================================  ;
;  handles                                                   ;
;=======================================================  ; 
section .text
install_handles:
	cli
	push eax
    push ecx
    push edx
    push ebx
    
    push ebp
    push esi
    push edi

    push ds
    push es
    push fs
    push gs
	mov eax, div_error ;hämta labelns adress
	mov ebx, div_error
	shr ebx, 16 ; high lagrat i bx
	
	mov  word [base_hi], bx
	mov  word [base_lo], ax
	
	mov eax, timer_handle
	mov ebx, timer_handle
	shr ebx, 16
	mov  word [base_hi_timer], bx
	mov  word [base_lo_timer], ax
			
	mov eax, gpf_error ;hämta labelns adress
	mov ebx, gpf_error
	shr ebx, 16 ; high lagrat i bx
	
	mov  word [base_hi_gpf], bx
	mov  word [base_lo_gpf], ax
	
	mov eax, kb_handle ;hämta labelns adress
	mov ebx, kb_handle
	shr ebx, 16 ; high lagrat i bx
	
	mov  word [base_hi_kb], bx
	mov  word [base_lo_kb], ax
	
	mov eax, page_fault ;hämta labelns adress
	mov ebx, page_fault
	shr ebx, 16 ; high lagrat i bx
	
	mov  word [base_hi_page], bx
	mov  word [base_lo_page], ax
	
	mov eax, rtl_net_handle ;hämta labelns adress
mov ebx, rtl_net_handle
	shr ebx, 16 ; high lagrat i bx
	
	mov  word [base_hi_rtl], bx
	mov  word [base_lo_rtl], ax
	
	mov dword [tx_desc], 0
		  pop gs
    pop fs
    pop es
    pop ds

    pop edi
    pop esi
    pop ebp
    pop ebx
    pop edx
    pop ecx
    pop eax
	sti
ret
