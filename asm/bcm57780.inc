broadcom:
	
	mov edx, 0x145000
	add edx, 0x10
	mov  eax, dword [edx]
	call puthex
	
	mov edx, 0x145000
	add edx, 0x6804
	mov dword [edx], 0x0
	
	mov edx, 0x145000
	add edx, 0x04
	mov word [edx], 0x0fff
	
	
	xor eax, eax
	mov eax,0x145000
	lea ebx, [eax + 0x0]
	mov ax, word [ebx]
	call puthex
	
	;xor eax, eax
	;mov edx, 0x145000
	;add edx, 0x06
	;mov  ax,word [edx]
	;call puthex
	

ret

broadcom_net_handle:
mov esi, msg
call putstring
iret
