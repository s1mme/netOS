[bits 32]
section .text
tick_counter:
db 0
;=======================================================  ;
;  Timer_handle: what do you want the timer to do?                                                 ;
;=======================================================  ; 
timer_handle:
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




	inc dword [tick_count]
    ;mov eax, dword [tick_count]

	
	mov eax, esp
	;call scheduler
	mov esp, eax
	
		mov    al, 0x20
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
iret                                           ;
	


   


;=======================================================  ;
;  Timer_install: how fast do we want it to tick?!                                                    ;
;=======================================================  ; 
timer_install:
                                
	   mov	ax,0x36         
	   out	0x43,ax	      
	   mov	ax,0x00    ; default is 0x0000 (65536) (18.2 per sec) 
	   out	0x40,ax         			            
	   mov	ax,0x20      ; change this to change tick count!
	   out	0x40,ax              

ret


;=======================================================  ;
;  delay: how long do we want to wait?                                                  ;
;=======================================================  ;
;IN: eax

delay:
	xor edx, edx
	mov ebx, 10
	div ebx	
	mov dword [time], eax
	cmp dword [time], 0
	jz .t
	jmp .next
.t:	
	mov dword [time], 1

	mov eax, dword [tick_count]
	add eax, dword [time]
.next:	
	cmp dword [tick_count], eax
	jl .next
	
ret
espoint dd 0
time dd 0
tick_count dd 0
timermsg db "Timer test!", 0 
