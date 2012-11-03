;=======================================================  ;
;  Keyboard handler                                                 ;
;=======================================================  ; 
section .bss

getchar: resb 100
stringbuffer: resb 100
section .data
kbdus: db 0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=','\t','q', 'w', 'e', 'r','t', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	'\', '`', '\b', 'z', 'x', 'c', 'v', 'b', 'n','m', ',', '.', '/',   0,	'*',0,' '
section .text
kb_handle:
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

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax    
	
	
                                     
    in al, 0x60   ; get key scancode!
    test  al,10000000b
	jnz .done
    movzx eax, al
    mov al, BYTE [eax + kbdus]
	
	movsx eax, al
	
	
	mov ecx, dword [buffercounter]
	mov [stringbuffer + ecx], al
	add dword [ buffercounter],1
	

	push ecx
	push eax
	mov eax, dword [ buffercounter]
	;call putint
	pop eax
	pop ecx
	
	mov ecx, 4
	mov dword [eax], "help"
	mov ebx, help
	call strcmp


	mov ecx, 6
	mov dword [eax], "reboot"
	mov ebx, reboot
	call strcmp
	
	mov ecx, 6
	mov dword [eax], "uptime"
	mov ebx, uptime
	call strcmp
		
	
	cmp al, '\n'
	je .enter
	jmp .next
.enter:
push eax
	mov dword [buffercounter], 0x0
	mov dword [stringbuffer+0], 0x0
	mov byte [stringbuffer+6], 0x0
	add byte [ypos], 1
	mov byte [xpos], 0
	push eax
	push ecx
	call scroll
	pop ecx
	pop eax
	pop eax
	mov esi, shelluser
	call putstring_nonewline
	jmp .done
.next:	
	cmp al, 'q'			;backspace!
	je .moveback
	jmp .comeback
.moveback:
	cmp byte [xpos], 0x0
	jnz .move
	jmp .comeback
.move:
	sub byte [xpos],2
.comeback:	
	
	
.endofline:	

	
	
	cmp byte [xpos], 80
	je .enter
	
	push eax
	push ecx
	call putchar
	
	pop ecx
	pop eax 
	.done:  
	
	
	.endofcall:
	mov al, 0x20
	mov dx, 0x20
	out dx, al      ; outb(0x20, 0x20)                 
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
	iret                                              
align 4

getvarchar: db 0
buffercounter: dd 0
